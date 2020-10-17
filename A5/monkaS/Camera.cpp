#include "Camera.h"
#include <algorithm>
#include "Record.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

Camera::Camera(int widthRes, int heightRes)
{
	this->widthRes = widthRes;
	this->heightRes = heightRes;
	renderedImage = new float[widthRes * heightRes * 3];
}

glm::vec3 Camera::ComputeRayColor(Scene *scene, glm::vec3 Origin, glm::vec3 Direction, float t0, float t1, int depth) {
	glm::vec3 c = glm::vec3(0.0f,0.0f,0.0f); 
	Record Curr; // Record structure  of the current ray 

	if (scene->hit(Origin, Direction, 0.0f, 99999999999.9f, Curr)) {
		
		glm::vec3 newNormal = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 Reflected = glm::vec3(0.0f,0.0f,0.0f);//New direction
		glm::vec3 oldView = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 hitPoint = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 shadowDirection = glm::vec3(0.0f, 0.0f, 0.0f);
		depth++;

		if (Curr.Type == 1) {//Sphere is the closest hit 

			hitPoint = Origin + Curr.t * Direction;
			newNormal = glm::normalize(hitPoint - Curr.Position);
			Reflected = glm::normalize(glm::reflect(Direction, newNormal));
			hitPoint += 0.001f * Reflected;//Self Intersection helper
		}
		else if (Curr.Type == 2) {//Plane is the closest hit 
			hitPoint = Origin + Curr.t * Direction;
			//oldView = glm::normalize(Origin - hitPoint);
			newNormal = Curr.Normal;
			Reflected = glm::normalize(glm::reflect(Direction, newNormal));
			hitPoint += 0.001f * Reflected; //Self Intersection helper
		}
		if (depth <= 3) {
			c += Curr.km*ComputeRayColor(scene, hitPoint, Reflected, 0.0001, 99999999999.9f, depth); //Recursive step
		}
			c += Curr.ka;
			for (int j = 0; j < scene->lights.size(); j++) {
				//shadow ray Direction
				Record Srec;
				shadowDirection = glm::normalize(scene->lights[j]->Position - hitPoint);
				float LightDistance = glm::length(scene->lights[j]->Position - hitPoint);
				

				if (!scene->hit(hitPoint, shadowDirection, 0.0001f, LightDistance, Srec)) {
					float LN = glm::dot(shadowDirection, newNormal);
					glm::vec3 E = glm::normalize(Eye - hitPoint);
					glm::vec3 R = glm::normalize(-glm::reflect(shadowDirection, newNormal)); //Reflected 
					glm::vec3 S = Curr.ks * (pow(std::max(0.0f, glm::dot(R, E)), Curr.s)); //Specular 
					c += scene->lights[j]->Color * (Curr.kd * std::max(0.0f, LN) + S);

				}
			}
		return c;
	}
	else {
		return c;
	}
	
}

void Camera::TakePicture(Scene *scene, float Color [][1200][3])
{	
	//The first Ray is computed with the plane, subsecuent planes are recursive and depend on the hit spot
	glm::vec3 C = Eye + ((glm::normalize(LookAt - Eye)) * focalD);
	//compute w
	glm::vec3 w = glm::normalize((Eye - C) / glm::length(Eye - C));
	//compute u
	glm::vec3 u = glm::normalize((glm::cross(Up, w)) / glm::length(glm::cross(Up, w)));
	//compute v
	glm::vec3 v = glm::normalize(glm::cross(w, u));
	//compute Pw
	float ly = 2.0 * focalD * glm::tan(glm::radians(fovy) / 2.0f);
	float Pw = (ly) / (heightRes);

	float lx = ly * widthRes / heightRes;
	//compute Origin
	
	glm::vec3 Origin = glm::vec3(C[0] - (lx / 2.0f), C[1] - (ly / 2.0f), C[2]);
	
	glm::vec3 PC;
	glm::vec3 c = glm::vec3(0.0f,0.0f,0.0f);
	for(int x = 0; x <widthRes; x++) {
		for (int y = 0; y < heightRes; y++) {
			//compute PC
			 PC = Origin +(x + 0.5f)*Pw*u + (y + 0.5f)*Pw*v;

			//compute Direction
			glm::vec3 Direction = glm::normalize(PC - Eye);
			c = ComputeRayColor(scene,Eye, Direction, 0.0f, 999999999.99f,0);
			
			Color[y][x][0] = c[0];
			Color[y][x][1] = c[1];
			Color[y][x][2] = c[2];
		}

	}
	
}
