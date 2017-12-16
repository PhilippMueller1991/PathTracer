#include "Scene.h"
#include "Raytracer.h"

#define IMAGE_DPI 72
#define IMAGE_WIDTH 800 //1600
#define IMAGE_HEIGHT 450 //900

#define RAYTRACER_MAX_BOUNCE 8
#define RAYTRACER_SAMPLES_PER_PIXEL 10

// Four spheres with high reflactence
void CreateDebugScene0(Scene* scene)
{
	// Materials
	// Can't be const because global predefined colors are not initialized on compile time
	Material matRed   (1, 2, 0, Color::red);
	Material matBlue  (1, 2, 0, Color::blue);
	Material matGreen (1, 2, 0, Color::green);
	Material matYellow(1, 2, 0, Color::yellow);

	// Objects
	scene->objects.push_back(new Sphere(0.8f * Vector3::right,	0.5f, matBlue));
	scene->objects.push_back(new Sphere(-0.8f * Vector3::right, 0.5f, matYellow));
	scene->objects.push_back(new Sphere(0.8f * Vector3::up,		0.5f, matRed));
	scene->objects.push_back(new Sphere(-0.8f* Vector3::up,		0.5f, matGreen));

	// Lights
	scene->lights.push_back(new Light(Vector3(0, 0, -3), Color::white, 1.0f));
}

// Plane scene
void CreateDebugScene1(Scene* scene)
{
	// Materials
	// Can't be const because global predefined colors are not initialized on compile time
	// General materials
	Material matGray (1, 1, 0, Color(0.5f, 0.5f, 0.5f));
	Material matRed  (1, 1, 0, Color::red);
	Material matBlue (1, 1, 0, Color::blue);
	Material matGreen(1, 1, 0, Color::green);
	Material matGlass(0, 0, 1, Color::white, Color::white, 1.81f);
	// Diffuse materials
	Material matDiffuseGray	 (1, 0, 0, Color(0.5f, 0.5f, 0.5f));
	Material matDiffuseRed	 (1, 0, 0, Color::red);
	Material matDiffuseBlue	 (1, 0, 0, Color::blue);
	Material matDiffuseGreen (1, 0, 0, Color::green);
	Material matDiffuseYellow(1, 0, 0, Color::yellow);

	// Scene objects
	// Small inner Cube
	const float c = 0.5f;
	Vector3 scale(1, 1, 1);
	const Rotation r = Rotation::EulerAngles(PI / 4.0f, 0, PI / 4.0f);
	// Normals have to point outwarts
	scene->objects.push_back(new Plane(r * (c * Vector3(0, -1, 0)), r * Rotation::EulerAngles(PI / 2.0f, 0, 0), scale,  matGray));
	scene->objects.push_back(new Plane(r * (c * Vector3(0, 1, 0)),  r * Rotation::EulerAngles(-PI / 2.0f, 0, 0), scale, matGray));
	scene->objects.push_back(new Plane(r * (c * Vector3(-1, 0, 0)), r * Rotation::EulerAngles(0, -PI / 2.0f, 0), scale, matRed));
	scene->objects.push_back(new Plane(r * (c * Vector3(1, 0, 0)),  r * Rotation::EulerAngles(0, PI / 2.0f, 0), scale, matBlue));
	scene->objects.push_back(new Plane(r * (c * Vector3(0, 0, 1)),  r * Rotation::EulerAngles(0, 0, 0), scale, matGreen));
	scene->objects.push_back(new Plane(r * (c * Vector3(0, 0, -1)), r * Rotation::EulerAngles(0, PI, 0), scale, matGreen));

	// Bigger outer cube
	scale *= 5;
	scene->objects.push_back(new Plane(Vector3(0, -5, 0),  Rotation::EulerAngles(-PI / 2.0f, 0, 0), Vector3(10, 20, 0), matDiffuseGray));
	scene->objects.push_back(new Plane(Vector3(0, 5, 0),   Rotation::EulerAngles(PI / 2.0f, 0, 0),	Vector3(10, 20, 0), matDiffuseGray));
	scene->objects.push_back(new Plane(Vector3(-5, 0, 0),  Rotation::EulerAngles(0, PI / 2.0f, 0),	Vector3(20, 10, 0), matDiffuseRed));
	scene->objects.push_back(new Plane(Vector3(5, 0, 0),   Rotation::EulerAngles(0, -PI / 2.0f, 0), Vector3(20, 10, 0), matDiffuseBlue));
	scene->objects.push_back(new Plane(Vector3(0, 0, 10),  Rotation::EulerAngles(0, PI, 0),			Vector3(10, 10, 0), matDiffuseGreen));
	scene->objects.push_back(new Plane(Vector3(0, 0, -10), Rotation::EulerAngles(0, 0, 0),			Vector3(10, 10, 0), matDiffuseYellow));

	// Shere
	//scene->objects.push_back(new Sphere(Vector3(0, 0, 0), 0.8f, matGlass));
	
	// Lights
	scene->lights.push_back(new Light(Vector3(1, 1, -3), Color::white, 1.2f));
}

// Box scene
void CreateDebugScene2(Scene* scene)
{
	// Materials
	// Can't be const because global predefined colors are not initialized on compile time
	Material matDiffuseGray(1, 0, 0, Color(0.5f, 0.5f, 0.5f), Color::white, 1.5f, 0.0f, &Texture(Texture::CHESS_BOARD, 0.5f));
	Material matDiffuseRed(1, 0, 0, Color::red);
	Material matDiffuseBlue(1, 0, 0, Color::blue);
	Material matDiffuseGreen(1, 0, 0, Color::green);
	Material matMirror(0, 1, 0, Color::red, Color::white, 1.0f, 0.8f);
	Material matMilkyMirror(2, 1, 0, Color::white, Color::white, 80.0f);
	Material matGlass(0, 0, 1, Color::white, Color::orange, 1.7f);

	// Scene objects
	const float c = 1.5f;
	const Vector3 scale(3, 3, 3);
	// Build box
	scene->objects.push_back(new Plane(Vector3(0, -1, 0),	  Rotation::EulerAngles(-PI / 2.0f, 0, 0), scale, matDiffuseGray));
	scene->objects.push_back(new Plane(c * Vector3(0, 1, 0),  Rotation::EulerAngles(PI / 2.0f, 0, 0), scale, matDiffuseGray));
	scene->objects.push_back(new Plane(c * Vector3(-1, 0, 0), Rotation::EulerAngles(0, PI / 2.0f, 0), scale, matDiffuseRed));
	scene->objects.push_back(new Plane(c * Vector3(1, 0, 0),  Rotation::EulerAngles(0, -PI / 2.0f, 0), scale, matDiffuseBlue));
	scene->objects.push_back(new Plane(c * Vector3(0, 0, 1),  Rotation::EulerAngles(0, PI, 0), scale, matDiffuseGreen));
	//Fill box
	scene->objects.push_back(new Sphere(Vector3(-0.5f, -0.5f, 0.5f), 0.5f, matMirror));
	scene->objects.push_back(new Sphere(Vector3(0.6f, -0.5f, -0.5f), 0.5f, matGlass));

	// Lights
	scene->lights.push_back(new Light(Vector3(0, 0.9f, 0), Color::white, 1.2f));
}

int main(int argc, char** argv)
{
	std::cout << "Rendering scene..." << std::endl;
	// Seed random number generator with current time
	srand(static_cast<unsigned>(time(0)));

	// Camera
	const Vector3 camPos(0, 0, -3);
	const Vector3 lookAt(0, 0, 0);
	const Vector3 camDir = (lookAt - camPos).Normalize();
	Camera cam = Camera(camPos, camDir, IMAGE_WIDTH, IMAGE_HEIGHT);

	// Scene
	Scene scene = Scene(cam);

	// Fill scene
	//CreateDebugScene0(&scene);	// Four spheres with high reflectance
	//CreateDebugScene1(&scene);	// Plane scene
	CreateDebugScene2(&scene);	// Box scene

	// Raytracer
	Raytracer rt(&scene);
	Raytracer::samplesPerPixel = RAYTRACER_SAMPLES_PER_PIXEL;
	Raytracer::maxBounces = RAYTRACER_MAX_BOUNCE;

	// Render raytraced image
	rt.Render(IMAGE_WIDTH, IMAGE_HEIGHT);

	// Cleanup
	scene.objects.clear();
	scene.lights.clear();


	return EXIT_SUCCESS;
}