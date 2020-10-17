#pragma once
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Scene.h"
#include "Record.h"


class Camera
{
public:
	Camera();
	~Camera();

	Camera(int widthRes, int heightRes);

	void TakePicture(Scene *scene, float Color[][1920][3]);
	float* GetRenderedImage() { return renderedImage; };
	glm::vec3 Eye = glm::vec3(0.0f, 0.0f, 7.0f);
	glm::vec3 LookAt = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	float fovy = 45.0f;
	float focalD = 1.0;
	glm::vec3 ComputeRayColor(Scene *scene, glm::vec3 Origin, glm::vec3 Direction, float t0, float t1, int level);
	
private:

	int widthRes;
	int heightRes;

	float *renderedImage;


};
