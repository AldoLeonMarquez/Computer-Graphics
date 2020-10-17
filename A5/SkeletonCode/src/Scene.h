#pragma once
#include <vector>
#include "Shape.h"
#include "Plane.h"
#include "Sphere.h"
#include "Light.h"
#include "Record.h"

class Scene
{
public:
	Scene() {};
	~Scene() {};
	std::vector<Shape*> shapes;
	std::vector<Light*> lights;
	bool hit(glm::vec3 Origin, glm::vec3 Direction, float t0, float t1, Record &rec) {
		float T = t1;
		
		for (int i = 0; i < shapes.size(); i++) {
			float t = shapes[i]->intersect(Origin, Direction, t0, t1, rec);
			
			if (t < T) {
				//Could have just copied a Record but mah 
				rec.Normal = shapes[i]->GetNormal();
				rec.Center = shapes[i]->GetCenter();
				glm::vec3 A = shapes[i]->Getka();
				rec.ka = shapes[i]->Getka();
				rec.kd = shapes[i]->Getkd();
				rec.ks = shapes[i]->Getks();
				rec.km = shapes[i]->Getkm();
				rec.Position = shapes[i]->GetPosition();
				rec.s = shapes[i]->Gets();
				rec.R = shapes[i]->GetR();
				rec.t = t;
				rec.Type = shapes[i]->getType();
				T = t;
			}
		}
		if (T < t1) {
			return true;
		}
		else {
			return false;
		}
	}

private:
	

};
