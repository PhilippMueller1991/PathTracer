#pragma once

#include "Vector3.h"
#include "SceneObject.h"
#include "Light.h"
#include "Camera.h"

#include <vector>

class Scene
{
public:
	Camera cam;
	std::vector<SceneObject*> objects;
	std::vector<Light*> lights;

	Scene(Camera cam) 
		: cam(cam)
	{
	}
	~Scene() 
	{
		objects.clear();
		lights.clear();
	}
};

