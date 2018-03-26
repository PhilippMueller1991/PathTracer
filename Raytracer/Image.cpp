#include "Image.h"

#include <iostream>

void Image::SetPixel(int idx, Color color)
{
	data[idx].r = color.r;
	data[idx].g = color.g;
	data[idx].b = color.b;
}

void Image::SaveBitmap(std::string filename)
{
	FILE* imageFile;

	int numPixels = width * height;
	int bitmapSize = 4 * numPixels;
	int filesize = 54 + bitmapSize;

	double factor = 39.375;
	int m = static_cast<int>(factor);
	int ppm = dpi * m;

	// BMP Header Details:
	// Offset	Size	Hex Value		Value						Description
	// 0 		2 		42 4D 			"BM" 						ID field(42h, 4Dh)
	// 2 		4 		46 00 00 00 	70 bytes(54 + 16) 			Size of the BMP file
	// 6 		2 		00 00 			Unused 						Application specific
	// 8 		2 		00 00 			Unused 						Application specific
	// A 		4 		36 00 00 00 	54 bytes(14 + 40) 			Offset where the pixel array(bitmap data) can be found
	unsigned char bmpFileHeader[14] = {
		'B','M',
		filesize, filesize >> 8, filesize >> 16, filesize >> 24,
		0,0,
		0,0,
		54,0,0,0
	};

	// DIB Header
	// Offset	Size	Hex Value		Value							Description
	//0E 		4 		28 00 00 00 	40 bytes 						Number of bytes in the DIB header(from this point)
	//12 		4 		02 00 00 00 	2 pixels(left to right order) 	Width of the bitmap in pixels
	//16 		4 		02 00 00 00 	2 pixels(bottom to top order) 	Height of the bitmap in pixels.Positive for bottom to top pixel order.
	//1A 		2 		01 00 			1 plane 						Number of color planes being used
	//1C 		2 		18 00 			24 bits 						Number of bits per pixel
	//1E 		4 		00 00 00 00 	0 								BI_RGB, no pixel array compression used
	//22 		4 		10 00 00 00 	16 bytes 						Size of the raw bitmap data(including padding)
	//26 		4 		13 0B 00 00 	2835 pixels / meter horizontal 	Print resolution of the image, 72 DPI × 39.3701 inches per meter yields 2834.6472
	//2A 		4 		13 0B 00 00 	2835 pixels / meter vertical	^
	//2E 		4 		00 00 00 00 	0 colors 						Number of colors in the palette
	//32 		4 		00 00 00 00 	0 important colors 				0 means all colors are important
	unsigned char bmpInfoHeader[40] = {
		40,0,0,0,
		width, width >> 8, width >> 16, width >> 24,
		height, height >> 8, height >> 16, height >> 24,
		1,0,
		24,0,
		0,0,0,0,
		bitmapSize, bitmapSize >> 8, bitmapSize >> 16, bitmapSize >> 24,
		ppm, ppm >> 8, ppm >> 16, ppm >> 24,
		ppm, ppm >> 8, ppm >> 16, ppm >> 24,
		0,0,0,0,
		0,0,0,0
	};

	imageFile = fopen(filename.c_str(), "wb");

	fwrite(bmpFileHeader, sizeof(unsigned char), 14, imageFile);
	fwrite(bmpInfoHeader, sizeof(unsigned char), 40, imageFile);

	for (int i = 0; i < numPixels; i++)
	{
		RGB pixel = data[i];
		unsigned char red   = static_cast<unsigned char>(pixel.r * 255.0f);
		unsigned char green = static_cast<unsigned char>(pixel.g * 255.0f);
		unsigned char blue  = static_cast<unsigned char>(pixel.b * 255.0f);

		unsigned char color[3] = { blue, green, red };	// Uses actually BGR
		fwrite(color, sizeof(unsigned char), 3, imageFile);
	}

	fclose(imageFile);
}