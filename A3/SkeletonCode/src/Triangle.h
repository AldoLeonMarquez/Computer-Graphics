#pragma once

#include <stdlib.h>
#include <math.h>

#include <glm/glm.hpp>


class Triangle {
private:
	// Triangle vertices


public:
	glm::vec3 v[3];
	glm::vec3 c[3];		// Vertex color
	// Constructor
	Triangle(glm::vec3 &v0, glm::vec3 &v1, glm::vec3 &v2);

	// Rendering the triangle using OpenGL
	void RenderOpenGL(glm::mat4 &modelViewMatrix, glm::mat4 &projectionMatrix);

	// Rendering the triangle using CPU
	void RenderCPU(glm::mat4 &projection, glm::mat4 &modelViewMatrix, glm::mat4 &viewPort, float color[][1000][3], float depth[][1000], bool Zmode,float zMax, float zMin);

	//
	bool insideT(float x, float y, glm::vec4 p1, glm::vec4 p2, glm::vec4 p3);
		
};