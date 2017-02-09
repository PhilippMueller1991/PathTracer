#include "Scene.h"
#include "Raytracer.h"

#define IMAGE_DPI 72
#define IMAGE_WIDTH 640
#define IMAGE_HEIGHT 640

#define RAYTRACER_MAX_BOUNCE 2
#define RAYTRACER_SAMPLES_PER_PIXEL 1


int main(int argc, char** argv)
{
	std::cout << "Rendering scene..." << std::endl;
	//######################### INITIALIZATION #########################//
	// Random number generator
	srand(static_cast<unsigned>(time(0)));

	// Camera
	const Vector3 camPos(0.0f, 1.0f, -2.5f);
	const Vector3 lookAt(0, 0, 0);
	const Vector3 camDir = (lookAt - camPos).Normalize();
	Camera cam = Camera(camPos, camDir, IMAGE_WIDTH, IMAGE_HEIGHT);

	// Scene
	Scene scene = Scene(cam);

	// Lights
	scene.lights.push_back(new Light(Vector3(2, 6, 2), Color(0.2f, 0.4f, 1.0f), 0.8f));
	scene.lights.push_back(new Light(Vector3(4, 3, -3), Color::white, 0.4f));

	// Materials
	// Can't be const because global predefined colors are not initialized to the compile time
	Material matDiffuseGray(1, 0, 0, Color(0.5f, 0.5f, 0.5f));
	Material matDiffuseGreen(1, 0, 0, Color::green);
	Material matMirror(0, 5, 0, Color::red, Color::white, 4.0f);
	Material matRed(3, 1, 0, Color::red);
	Material matBlue(5, 1, 0, Color::blue);
	Material matYellow(2, 2, 0, Color::yellow);
	Material matOrange(1, 5, 0, Color::orange);

	// Scene objects
	scene.objects.push_back(new Sphere(Vector3(0.4f, -0.5f, 2.0f), 0.5f, matMirror));
	scene.objects.push_back(new Sphere(Vector3(-0.3f, -0.75f, 0.8f), 0.25f, matBlue));
	scene.objects.push_back(new Sphere(Vector3(0, 0.5f, 0), 0.2f, matYellow));
	scene.objects.push_back(new Sphere(Vector3(0.1f, 1, 0), 0.2f, matDiffuseGray));
	scene.objects.push_back(new Plane(Vector3(0, -1, 0), Vector3::up, matDiffuseGreen));

	// Raytracer
	Raytracer rt;
	rt.scene = &scene;
	Raytracer::samplesPerPixel = RAYTRACER_SAMPLES_PER_PIXEL;
	Raytracer::maxBounces = RAYTRACER_MAX_BOUNCE;

	//######################### RENDER RAYTRACED IMAGE #########################//
	rt.Render(IMAGE_WIDTH, IMAGE_HEIGHT);

	return EXIT_SUCCESS;
}