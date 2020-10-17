#pragma once
#include "Shape.h"
#include <glm/glm.hpp>
#include <algorithm>

class Sphere :
	public Shape
{
public:
	Sphere() {};
	~Sphere() {};
	
	int Type = 1;
	glm::vec3 Position;
	glm::vec3 ka;
	glm::vec3 kd;
	glm::vec3 ks;
	glm::vec3 km;
	float R;
	float s;
	glm::vec3 Center; //useless
	glm::vec3 Normal; //useless

	virtual float intersect(glm::vec3 Origin, glm::vec3 Direction, float t0, float t1, Record &rec) {
		
		float t = t1;;
		//std::cout << Direction[0] << "  CMON TARS " << Direction[1] << " " << Direction[2] << std::endl;
		float a = glm::dot(Direction, Direction);
		float b = glm::dot(2.0f * (Origin - Position),Direction);
		float c = glm::dot((Origin - Position), (Origin - Position)) - pow(R, 2);
		float bb = pow(b, 2);
		float ac = 4.0f*a*c;
		if (bb - ac < 0.0f) {
			return t;
		}
		float root = pow(bb - ac, 0.5f);
		//std::cout << root << "   " << ac << std::endl;
		float t3 = (-b + root) / 2.0f * a; 
		float t4 = (-b - root) / 2.0f * a; 
		
		if (t3 > t0 && t3 < t1) {
			if (t4 > t0 && t4 < t1) {
				t = std::min(t3, t4);
				return t;
			}
			else {
				t = t3;
				return t3;
			}
		}
		return t;
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
