#include "Scene.h"
#include "Raytracer.h"

#define IMAGE_DPI 72
#define IMAGE_WIDTH 640
#define IMAGE_HEIGHT 640

#define RAYTRACER_MAX_BOUNCE 1

// Materials
// Can't be const because global predefined colors are not initialized to the compile time
Material matGreen(1, 0, 0, Color::green);
Material matRed(2, 1, 0, Color::red);
Material matBlue(5, 1, 0, Color::blue);

int main(int argc, char** argv)
{
	std::cout << "Rendering scene..." << std::endl;
	//######################### INITIALIZATION #########################//
	// Random number generator
	srand(static_cast<unsigned>(time(0)));
		
	// Scene
	Scene scene = Scene();
	
	// Camera
	const Vector3 camPos(0.0f, 1.0f, -4.0f);
	const Vector3 lookAt(0, 0, 0);
	const Vector3 camDir = (lookAt - camPos).Normalize();
	scene.cam = Camera(camPos, camDir);

	// Lights
	scene.lights.push_back(new Light(Vector3(2, 6, 2), Color::white));

	// Scene objects
	scene.objects.push_back(new Sphere(Vector3(0.4f, -0.5f, 2.0f), 0.5f, matRed));
	scene.objects.push_back(new Sphere(Vector3(-0.3f, -0.75f, 0.8f), 0.25f, matBlue));
	scene.objects.push_back(new Sphere(Vector3(0, 0.5f, 0), 0.2f, matRed));
	scene.objects.push_back(new Sphere(Vector3(0.1f, 1, 0), 0.2f, matBlue));
	scene.objects.push_back(new Plane(Vector3(0, -1, 0), Vector3::up, matGreen));

	// Raytracer
	Raytracer rt;
	rt.scene = &scene;
	Raytracer::maxBounces = RAYTRACER_MAX_BOUNCE;

	//######################### RENDER RAYTRACED IMAGE #########################//
	rt.Render(IMAGE_WIDTH, IMAGE_HEIGHT);

	return EXIT_SUCCESS;
}