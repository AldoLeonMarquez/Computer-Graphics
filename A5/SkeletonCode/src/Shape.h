#pragma once
#include<iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "Record.h"

class Shape
{
public:
	
	Shape(void) {};
	~Shape(void) {};
	//pure virtual functions
	virtual float intersect(glm::vec3 Origin, glm::vec3 Direction, float t0, float t1, Record  &rec) = 0;
	virtual glm::vec3 GetNormal() =0;
	virtual glm::vec3 GetCenter() = 0;
	virtual glm::vec3 Getka() = 0;
	virtual glm::vec3 Getkd() = 0;
	virtual glm::vec3 Getks() = 0;
	virtual glm::vec3 Getkm() = 0;
	virtual glm::vec3 GetPosition() = 0;
	virtual float Gets() = 0;
	virtual float GetR() = 0;
	virtual int getType() = 0;
private:
	
};
