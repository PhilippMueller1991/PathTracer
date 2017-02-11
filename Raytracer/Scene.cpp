#include "Scene.h"
#include "Raytracer.h"

#define IMAGE_DPI 72
#define IMAGE_WIDTH 800
#define IMAGE_HEIGHT 450

#define RAYTRACER_MAX_BOUNCE 6
#define RAYTRACER_SAMPLES_PER_PIXEL 1

// TODO: Fix render of vertical image (Height > Width)
int main(int argc, char** argv)
{
	std::cout << "Rendering scene..." << std::endl;
	// Random number generator
	srand(static_cast<unsigned>(time(0)));

	// Camera
	const Vector3 camPos(0, 0, -3);
	const Vector3 lookAt(0, 0, 0);
	const Vector3 camDir = (lookAt - camPos).Normalize();
	Camera cam = Camera(camPos, camDir, IMAGE_WIDTH, IMAGE_HEIGHT);

	// Scene
	Scene scene = Scene(cam);

	// Lights
	scene.lights.push_back(new Light(Vector3(0, 0.9f, 0), Color::white, 1.2f));

	// Materials
	// Can't be const because global predefined colors are not initialized to the compile time
	Material matDiffuseGray(1, 0, 0, Color(0.5f, 0.5f, 0.5f));
	Material matDiffuseGreen(1, 0, 0, Color::green);
	Material matDiffuseRed(1, 0, 0, Color::red);
	Material matDiffuseBlue(1, 0, 0, Color::blue);
	Material matMirror(0, 5, 0, Color::red, Color::white, 80.0f);
	Material matRed(3, 1, 0, Color::red);
	Material matBlue(5, 1, 0, Color::blue);
	Material matYellow(2, 2, 0, Color::yellow);
	Material matOrange(1, 5, 0, Color::orange);
	Material matGlass(0.2f, 0, 1, Color::white, Color::white, 10.0f, 1.2f);

	// Scene objects
	// Build box
	scene.objects.push_back(new Plane(Vector3(-1.4f, 0, 0), Vector3(1, 0, 0), matDiffuseRed));
	scene.objects.push_back(new Plane(Vector3(1.4f, 0, 0), Vector3(-1, 0, 0), matDiffuseBlue));
	scene.objects.push_back(new Plane(Vector3(0, 1, 0), Vector3(0, -1, 0), matDiffuseGray));
	scene.objects.push_back(new Plane(Vector3(0, -1, 0), Vector3(0, 1, 0), matDiffuseGray));
	scene.objects.push_back(new Plane(Vector3(0, 0, 1), Vector3(0, 0, -1), matDiffuseGray));
	scene.objects.push_back(new Plane(Vector3(0, 0, -5), Vector3(0, 0, 1), matDiffuseGray));
	// Fill box
	scene.objects.push_back(new Sphere(Vector3(-0.5f, -0.5f, 0.5f), 0.5f, matMirror));
	scene.objects.push_back(new Sphere(Vector3(0.6f, -0.5f, -0.5f), 0.5f, matGlass));

	// Raytracer
	Raytracer rt;
	rt.scene = &scene;
	Raytracer::samplesPerPixel = RAYTRACER_SAMPLES_PER_PIXEL;
	Raytracer::maxBounces = RAYTRACER_MAX_BOUNCE;

	//######################### RENDER RAYTRACED IMAGE #########################//
	rt.Render(IMAGE_WIDTH, IMAGE_HEIGHT);

	return EXIT_SUCCESS;
}