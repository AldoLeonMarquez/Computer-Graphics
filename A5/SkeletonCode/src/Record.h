#pragma once
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

struct Record
{
	//std::vector <int> t;
	float t = 0.0f;
	int Type = 0;
	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 Center = glm::vec3(0.0f, 0.0f, 0.0f);//If type is 2, i.e a plane 
	glm::vec3 Normal = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 ka = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 kd = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 ks = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 km = glm::vec3(0.0f, 0.0f, 0.0f);
	float R = 0.0f;
	float s = 0.0f;


};