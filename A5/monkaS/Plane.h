#pragma once
#include "Shape.h"
#include <glm/glm.hpp>
#include "Record.h"

class Plane :
	public Shape
{
public:
	Plane() {};
	~Plane() {};
	int Type = 2;
	glm::vec3 Center;
	glm::vec3 Normal;
	glm::vec3 ka;
	glm::vec3 kd;
	glm::vec3 ks;
	glm::vec3 km;
	float s;
	float R;//useless
	glm::vec3 Position;//useless
	

	
	virtual float intersect(glm::vec3 Origin, glm::vec3 Direction, float t0, float t1, Record &rec) {
		float t = t1;
		float t2 = (glm::dot((Center - Origin), Normal)) / ( glm::dot(Direction,Normal));
		//std::cout << t2 << std::endl;
		if (t2 < t0) {
			return t;
		}
		else {
			//std::cout << t2 << std::endl;
			t = t2;
			return t;
		}
		
	};

	virtual glm::vec3 GetNormal() { return Normal; };
	virtual glm::vec3 GetCenter() { return Center; };
	virtual glm::vec3 Getka() { return ka; };
	virtual glm::vec3 Getkd() { return kd; };
	virtual glm::vec3 Getks() { return ks; };
	virtual glm::vec3 Getkm() { return km; };
	virtual glm::vec3 GetPosition() { return Position; };
	virtual float Gets() { return s; };
	virtual float GetR() { return R; };
	virtual int getType() { return Type; };
	
private:

};
