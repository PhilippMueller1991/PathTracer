#include <iostream>
#include <math.h>
#include <string>
#include <fstream>
#include <vector>
#include <time.h>

#include "Vector3.h"
#include "Camera.h"
#include "Ray.h"
#include "Light.h"
#include "Sphere.h"
#include "Plane.h"

#define IMAGE_DPI 72
#define IMAGE_WIDTH 640
#define IMAGE_HEIGHT 640

#define RAYTRACER_MAX_BOUNCE 1


struct RGB {
public:
	// RBG values have to be normalized
	float r;
	float g;
	float b;

	RGB() {}
	RGB(float x, float y, float z) : r(x), g(y), b(z) {}
};

struct Intersection
{
	int idx;
	float distance;

	Intersection(int idx, float distance) : idx(idx), distance(distance) {}
};

void SaveImageAsBitmap(std::string filename, int width, int height, int dpi, const RGB *data)
{
	FILE * imageFile;

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
	//26 		4 		13 0B 00 00 	2835 pixels / meter horizontal 	Print resolution of the image, 72 DPI � 39.3701 inches per meter yields 2834.6472
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
		int red = static_cast<int>(pixel.r * 255.0f);
		int green = static_cast<int>(pixel.g * 255.0f);
		int blue = static_cast<int>(pixel.b * 255.0f);

		unsigned char color[3] = { blue, green, red };	// Uses actually BGR
		fwrite(color, sizeof(unsigned char), 3, imageFile);
	}

	fclose(imageFile);
}

Intersection ComputeFirstRayObjectIntersection(const Ray& camRay, const std::vector<SceneObject*>& objects)
{
	Intersection nearestIntersection(-1, FLT_MAX);
	for (uint32_t i = 0; i < objects.size(); i++)
	{
		float intersectionDistance = objects[i]->findIntersection(camRay);
		if (intersectionDistance > 0 && intersectionDistance < nearestIntersection.distance)
		{
			nearestIntersection.distance = intersectionDistance;
			nearestIntersection.idx = i;
		}
	}

	return nearestIntersection;
}

Color EvaluateLocalLightingModel(Vector3 normal, Material mat)
{
	// TODO
	// FOR DEBUG RETURN MATERIAL COLOR
	return mat.color;
}

int IsInShadow(const Vector3& camPos, const Vector3& hitPos, const std::vector<SceneObject*>& objects)
{
	// TODO
	return 0;
}

Color Traverse(const Ray& ray, const std::vector<SceneObject*>& objects)
{
	Intersection intersection = ComputeFirstRayObjectIntersection(ray, objects);
	
	// No object was hit, return background color
	if (intersection.idx < 0)
		return Color(1, 1, 1, 0);

	// Pathtracer: Randomly choose one of the following rays: 
	// { normal, reflection, refraction }
	Vector3 normal;
	// TODO: cast shadow ray at each ray object intersection
	Vector3 hitPos = ray.direction * intersection.distance + ray.origin;
	Color color;
	if (!IsInShadow(hitPos, hitPos, objects))
		color = EvaluateLocalLightingModel(normal, objects[intersection.idx]->material);
	else
		color = 0.2f * EvaluateLocalLightingModel(normal, objects[intersection.idx]->material);

	// Pathtracer: Needs additional material property diffuse in recursive call
	//Material mat;
	//return color + mat.ks * Traverse(reflect) + mat.kt * Traverse(refract);
	return color;
}

//############################## PSEUDEO ALGORITHM ##############################
//foreach(pixel)
//{
//	ray = GetRay(viewPoint, pixelCenter)
//	pixel color = Traverse(ray)
//}
//Traverse(ray)
//{
//	compute FirstRay-ObjectIntersection
//	compute normal, reflection and refraction ray
//	color = EvaluateLocalLightingModel(normal)
//	return color + ks * Traverse(reflect) + kt * Traverse(refract)
//}
//###############################################################################
int main(int argc, char** argv)
{
	std::cout << "Rendering scene..." << std::endl;

	//###################### INITIALIZATION ######################
	// Random number generator
	srand(static_cast<unsigned>(time(0)));
	// Raytracer
	Ray::maxBounces = RAYTRACER_MAX_BOUNCE;
	// Image Settings
	const float aspectRatio = (float)IMAGE_WIDTH / (float)IMAGE_HEIGHT;
	// Camera
	const Vector3 camPos(3.0f, 1.5f, -4.0f);
	const Vector3 lookAt(0, 0, 0);
	const Vector3 camDir = (lookAt - camPos).Normalize();
	Camera cam = Camera(camPos, camDir);
	// Colors
	const Color white(1, 1, 1, 0);
	const Color greenish(0.3f, 1.0f, 0.5f, 0.3f);
	const Color red(1.0f, 0.1f, 0.2f, 0.0f);
	// Materials
	const Material matGreenish(1, 2, 1, greenish);
	const Material matRed(2, 1, 1, red);
	// Lights
	const Light pointLight(Vector3(-7, 10, -10), white);
	// Scene objects
	std::vector<SceneObject*> sceneObjects;
	sceneObjects.push_back(new Sphere(Vector3(0, 0, 0), 0.5f, matGreenish));
	sceneObjects.push_back(new Plane(-Vector3(0, 1, 0), Vector3::up, matRed));

	//###################### GENERATE IMAGE ######################
	RGB *pixels = new RGB[IMAGE_WIDTH * IMAGE_HEIGHT];
	for (int y = 0; y < IMAGE_HEIGHT; y++)
	{
		for (int x = 0; x < IMAGE_WIDTH; x++)
		{
			int idx = y * IMAGE_WIDTH + x;

			// TODO: Accomodate for aspectRatio
			// TODO: Shoot multiple rays with random jitter (later Sobol jitter, or Multi-Jittered) for AA
			Vector3 rayDir = cam.PixelToRayDir(x, y, IMAGE_WIDTH, IMAGE_HEIGHT);
			Ray camRay(cam.pos, rayDir);
			Color color = Traverse(camRay, sceneObjects);
			pixels[idx].r = color.r;
			pixels[idx].g = color.g;
			pixels[idx].b = color.b;
		}
	}

	//###################### SAVE IMAGE ######################
	std::cout << "Writing scene.bmp" << std::endl;
	SaveImageAsBitmap("..//scene.bmp", IMAGE_WIDTH, IMAGE_HEIGHT, IMAGE_DPI, pixels);
	// Cleanup
	delete pixels;
	sceneObjects.clear();
	
	return EXIT_SUCCESS;
}