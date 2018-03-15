#include "Scene.h"
#include "Raytracer.h"
#include "TextureChessPattern.h"

#include <chrono>
#include <synchronizationerrors.h>

#define IMAGE_DPI 72
#define IMAGE_WIDTH 1600 / 2
#define IMAGE_HEIGHT 900 / 2

#define RAYTRACER_MAX_BOUNCE 8 / 8
#define RAYTRACER_SAMPLES_PER_PIXEL 32 / 32

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
	scene->objects.push_back(new Sphere(0.8f * Vec3f::right,	0.5f, matBlue));
	scene->objects.push_back(new Sphere(-0.8f * Vec3f::right, 0.5f, matYellow));
	scene->objects.push_back(new Sphere(0.8f * Vec3f::up,		0.5f, matRed));
	scene->objects.push_back(new Sphere(-0.8f* Vec3f::up,		0.5f, matGreen));

	// Lights
	scene->lights.push_back(new Light(Vec3f(0, 0, -3), Color::white, 1.0f));
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
	Vec3f scale(1, 1, 1);
	const Rotation r = Rotation::eulerAngles(PI / 4.0f, 0, PI / 4.0f);
	// Normals have to point outwarts
	scene->objects.push_back(new Plane(r * (c * Vec3f(0, -1, 0)), r * Rotation::eulerAngles(PI / 2.0f, 0, 0), scale,  matGray));
	scene->objects.push_back(new Plane(r * (c * Vec3f(0, 1, 0)),  r * Rotation::eulerAngles(-PI / 2.0f, 0, 0), scale, matGray));
	scene->objects.push_back(new Plane(r * (c * Vec3f(-1, 0, 0)), r * Rotation::eulerAngles(0, -PI / 2.0f, 0), scale, matRed));
	scene->objects.push_back(new Plane(r * (c * Vec3f(1, 0, 0)),  r * Rotation::eulerAngles(0, PI / 2.0f, 0), scale, matBlue));
	scene->objects.push_back(new Plane(r * (c * Vec3f(0, 0, 1)),  r * Rotation::eulerAngles(0, 0, 0), scale, matGreen));
	scene->objects.push_back(new Plane(r * (c * Vec3f(0, 0, -1)), r * Rotation::eulerAngles(0, PI, 0), scale, matGreen));

	// Bigger outer cube
	scale *= 5;
	scene->objects.push_back(new Plane(Vec3f(0, -5, 0),  Rotation::eulerAngles(-PI / 2.0f, 0, 0), Vec3f(10, 20, 0), matDiffuseGray));
	scene->objects.push_back(new Plane(Vec3f(0, 5, 0),   Rotation::eulerAngles(PI / 2.0f, 0, 0),	Vec3f(10, 20, 0), matDiffuseGray));
	scene->objects.push_back(new Plane(Vec3f(-5, 0, 0),  Rotation::eulerAngles(0, PI / 2.0f, 0),	Vec3f(20, 10, 0), matDiffuseRed));
	scene->objects.push_back(new Plane(Vec3f(5, 0, 0),   Rotation::eulerAngles(0, -PI / 2.0f, 0), Vec3f(20, 10, 0), matDiffuseBlue));
	scene->objects.push_back(new Plane(Vec3f(0, 0, 10),  Rotation::eulerAngles(0, PI, 0),			Vec3f(10, 10, 0), matDiffuseGreen));
	scene->objects.push_back(new Plane(Vec3f(0, 0, -10), Rotation::eulerAngles(0, 0, 0),			Vec3f(10, 10, 0), matDiffuseYellow));

	// Shere
	//scene->objects.push_back(new Sphere(Vec3f(0, 0, 0), 0.8f, matGlass));
	
	// Lights
	scene->lights.push_back(new Light(Vec3f(1, 1, -3), Color::white, 1.2f));
}

// Box scene
void CreateDebugScene2(Scene* scene)
{
	// Textures
	std::shared_ptr<Texture> texChessBoard = std::make_shared<TextureChessPattern>(10.0f, Color::black, Color::white);

	// Materials
	// Can't be const because global predefined colors are not initialized on compile time
	Material matDiffuseTexture(1, 0, 0, Color(0.5f, 0.5f, 0.5f), Color::white, 1.5f, 0.0f, texChessBoard);
	Material matDiffuseGray(1, 0, 0, Color(0.5f, 0.5f, 0.5f), Color::white, 1.5f, 0.0f);
	Material matDiffuseRed(1, 0, 0, Color::red);
	Material matDiffuseBlue(1, 0, 0, Color::blue);
	Material matDiffuseGreen(1, 0, 0, Color::green);
	Material matMirror(0, 1, 0, Color::red, Color::white, 1.0f, 0.02f);
	Material matRoughMirror(5, 1, 4, Color::yellow, Color::white, 1.0f, 0.22f);
	Material matGlass(0, 0, 1, Color::white, Color::white, 1.7f, 0.0f);
	Material matRoughGlass(0, 0, 1, Color::white, Color::white, 1.9f, 0.35f);

	// Scene objects
	const float c = 1.5f;
	const Vec3f scale(3, 3, 3);
	// Build box
	scene->objects.push_back(new Plane(Vec3f(0, -1, 0),	  Rotation::eulerAngles(-PI / 2.0f, 0, 0), scale, matDiffuseTexture));
	scene->objects.push_back(new Plane(c * Vec3f(0, 1, 0),  Rotation::eulerAngles(PI / 2.0f, 0, 0), scale, matDiffuseGray));
	scene->objects.push_back(new Plane(c * Vec3f(-1, 0, 0), Rotation::eulerAngles(0, PI / 2.0f, 0), scale, matDiffuseRed));
	scene->objects.push_back(new Plane(c * Vec3f(1, 0, 0),  Rotation::eulerAngles(0, -PI / 2.0f, 0), scale, matDiffuseBlue));
	scene->objects.push_back(new Plane(c * Vec3f(0, 0, 1),  Rotation::eulerAngles(0, PI, 0), scale, matDiffuseGreen));
	//Fill box
	scene->objects.push_back(new Sphere(Vec3f(-0.5f, -0.5f, 0.5f), 0.5f, matMirror));
	scene->objects.push_back(new Sphere(Vec3f(0.6f, -0.5f, -0.5f), 0.5f, matGlass));
	scene->objects.push_back(new Sphere(Vec3f(-0.2f, -0.7f, -0.7f), 0.3f, matRoughMirror));

	// Lights
	scene->lights.push_back(new Light(Vec3f(0, 0.9f, 0), Color::white, 1.2f));
	scene->lights.push_back(new Light(Vec3f(0.3f, 0.6f, -2.0f), Color::white, 0.6f));
}

// Box with mutliple glass spheres
void CreateDebugScene3(Scene* scene)
{
	// Materials
	// Can't be const because global predefined colors are not initialized on compile time
	Material matDiffuseGray(1, 0, 0, Color(0.5f, 0.5f, 0.5f), Color::white, 1.5f, 0.0f);
	Material matDiffuseRed(1, 0, 0, Color::red);
	Material matDiffuseBlue(1, 0, 0, Color::blue);
	Material matDiffuseGreen(1, 0, 0, Color::green);
	Material matMirror(0, 1, 0, Color::red, Color::white, 1.0f, 0.22f);
	Material matGlass(0.4f, 0, 1, Color::white, Color::orange, 1.57f, 0.0f);

	// Scene objects
	const float c = 1.5f;
	const Vec3f scale(3, 3, 3);
	// Build box
	scene->objects.push_back(new Plane(c * Vec3f(0, -1, 0), Rotation::eulerAngles(-PI / 2.0f, 0, 0), scale, matDiffuseGray));
	scene->objects.push_back(new Plane(c * Vec3f(0, 1, 0), Rotation::eulerAngles(PI / 2.0f, 0, 0), scale, matDiffuseGray));
	scene->objects.push_back(new Plane(c * Vec3f(-1, 0, 0), Rotation::eulerAngles(0, PI / 2.0f, 0), scale, matDiffuseRed));
	scene->objects.push_back(new Plane(c * Vec3f(1, 0, 0), Rotation::eulerAngles(0, -PI / 2.0f, 0), scale, matDiffuseBlue));
	scene->objects.push_back(new Plane(c * Vec3f(0, 0, 1), Rotation::eulerAngles(0, PI, 0), scale, matDiffuseGreen));
	//Fill box
	float start = 0.9f;
	const float offset = 0.5f;
	scene->objects.push_back(new Sphere(Vec3f(0, 0, start - offset), 0.4f, matGlass));
	scene->objects.push_back(new Sphere(Vec3f(0, 0, start - 2 * offset), 0.3f, matGlass));
	scene->objects.push_back(new Sphere(Vec3f(0, 0, start - 3 * offset), 0.2f, matGlass));
	scene->objects.push_back(new Sphere(Vec3f(0, 0, start - 4 * offset), 0.1f, matGlass));

	// Lights
	//scene->lights.push_back(new Light(Vec3f(0, 0.9f, 0), Color::white, 1.2f));
	//scene->lights.push_back(new Light(Vec3f(0, 0, 1.2f), Color::white, 1.2f));
	scene->lights.push_back(new Light(Vec3f(0, 0, -5.5f), Color::white, 1.2f));
}

// Large scene
void CreateDebugScene4(Scene* scene)
{
	// Materials
	// Can't be const because global predefined colors are not initialized on compile time
	Material matDiffuseGray(1, 0, 0, Color(0.5f, 0.5f, 0.5f), Color::white, 1.5f, 0.0f);
	Material matMirror(0, 1, 0, Color::red, Color::white, 1.0f, 0.22f);
	Material matMirrorRough(0, 1, 0, Color::white, Color::white, 1.0f, 8.82f);
	Material matGlass(0.4f, 0, 1, Color::white, Color::orange, 1.57f, 0.0f);

	// Scene objects
	const Vec3f scale(80, 80, 80);
	// Build box
	scene->objects.push_back(new Plane(Vec3f(0, -1, 0), Rotation::eulerAngles(-PI / 2.0f, 0, 0), scale, matDiffuseGray));
	// Place spheres on plane
	float start = 0.9f;
	const float offset = 0.5f;
	const float randScale = 0.25f;
	for (int i = -10; i < 10; i++)
	{
		for (int j = -10; j < 10; j++)
		{
			scene->objects.push_back(new Sphere(Vec3f(static_cast<float>(i), -0.7f, static_cast<float>(j)),
				0.3f,
				Material((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX,
					Color::getRandomColor(),
					Color::getRandomColor(),
					(rand() % 2 / 2.0f),
					(float)rand() / (float)RAND_MAX)));
		}
	}
	// Lights
	for (int i = -4; i < 4; i++)
	{
		scene->lights.push_back(new Light(Vec3f(rand() / static_cast<float>(RAND_MAX) - 0.5f,
			0.9f,
			static_cast<float>(i * 3.5f)),
			Color::white,
			2.8f));
	}
}

// OBJ model scene
void CreateDebugScene5(Scene* scene)
{
	// Textures
	std::shared_ptr<Texture> texChessBoard = std::make_shared<TextureChessPattern>(10.0f, Color::black, Color::white);

	// Materials
	// Can't be const because global predefined colors are not initialized on compile time
	Material matDiffuseTexture(1, 0, 0, Color(0.5f, 0.5f, 0.5f), Color::white, 1.5f, 0.0f, texChessBoard);
	Material matDiffuseGray(1, 0, 0, Color(0.5f, 0.5f, 0.5f), Color::white, 1.5f, 0.0f);
	Material matDiffuseWhite(1, 0, 0, Color(1.0f, 1.0f, 1.0f), Color::white, 1.5f, 0.5f);
	Material matDiffuseRed(1, 0, 0, Color::red);
	Material matDiffuseBlue(1, 0, 0, Color::blue);
	Material matDiffuseGreen(1, 0, 0, Color::green);
	Material matSpecularWhite(1, 1.5f, 0, Color(1.0f, 1.0f, 1.0f), Color::white, 1.5f, 0.5f);
	Material matMirror(0, 1, 0, Color::red, Color::white, 1.0f, 0.22f);
	Material matGlass(0.4f, 0, 1, Color::white, Color::orange, 1.57f, 0.0f);

	// Scene objects
	const float c = 1.5f;
	const Vec3f scale(3, 3, 3);
	// Build box
	scene->objects.push_back(new Plane(Vec3f(0, -1, 0), Rotation::eulerAngles(-PI / 2.0f, 0, 0), scale, matDiffuseTexture));
	scene->objects.push_back(new Plane(c * Vec3f(0, 1, 0), Rotation::eulerAngles(PI / 2.0f, 0, 0), scale, matDiffuseGray));
	scene->objects.push_back(new Plane(c * Vec3f(-1, 0, 0), Rotation::eulerAngles(0, PI / 2.0f, 0), scale, matDiffuseRed));
	scene->objects.push_back(new Plane(c * Vec3f(1, 0, 0), Rotation::eulerAngles(0, -PI / 2.0f, 0), scale, matDiffuseBlue));
	scene->objects.push_back(new Plane(c * Vec3f(0, 0, 1), Rotation::eulerAngles(0, PI, 0), scale, matDiffuseGreen));
	//Fill box
	Mesh *suzanne = new Mesh(Vec3f::zero, Rotation::eye(), Vec3f::one, matDiffuseWhite);
	//suzanne->loadOBJ("..//plane.obj");
	suzanne->loadOBJ("..//halfCube.obj");
	//suzanne->loadOBJ("..//cube.obj");
	//suzanne->loadOBJ("..//suzanne.obj");
	suzanne->printBuffers();
	scene->objects.push_back(suzanne);

	// Lights
	scene->lights.push_back(new Light(Vec3f(0, 0, -3.5f), Color::white, 1.2f));
}

int main(int argc, char** argv)
{
	// Seed random number generator with current time
	srand(static_cast<unsigned int>(time(0)));

	// Camera
	//const Vec3f camPos(0.5f, 1.0f, -3.0f);
	const Vec3f camPos(-1.0f, 0.0f, -3.0f);
	const Vec3f lookAt(0.5f, 0, 0);
	const Vec3f camDir = (lookAt - camPos).normalize();
	Camera cam = Camera(camPos, camDir, IMAGE_WIDTH, IMAGE_HEIGHT);

	// Scene
	Scene scene = Scene(cam);

	// Fill scene
	//CreateDebugScene0(&scene);	// Four spheres with high reflectance
	//CreateDebugScene1(&scene);	// Plane scene
	//CreateDebugScene2(&scene);	// Box scene
	//CreateDebugScene3(&scene);	// Box with mutliple glass spheres
	//CreateDebugScene4(&scene);	// Large scene
	CreateDebugScene5(&scene);	// OBJ model scene

	// Raytracer
	Raytracer rt(&scene);
	Raytracer::samplesPerPixel = RAYTRACER_SAMPLES_PER_PIXEL;
	Raytracer::maxBounces = RAYTRACER_MAX_BOUNCE;

	// Render raytraced image
	std::cout << "Rendering scene..." << std::endl;
	auto begin = std::chrono::high_resolution_clock::now();
	rt.render(IMAGE_WIDTH, IMAGE_HEIGHT);
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
	std::cout << duration << " ms total." << std::endl;
	std::cout << duration / 1000 << " s total." << std::endl;
	system("pause");

	return EXIT_SUCCESS;
}