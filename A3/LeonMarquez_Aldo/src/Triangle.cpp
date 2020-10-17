#include "Triangle.h"
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <iostream>

// A function clamping the input values to the lower and higher bounds
#define CLAMP(in, low, high) ((in) < (low) ? (low) : ((in) > (high) ? (high) : in))


Triangle::Triangle(glm::vec3 &v0, glm::vec3 &v1, glm::vec3 &v2)
{
	v[0] = v0;
	v[1] = v1;
	v[2] = v2;

	c[0] = glm::vec3(1.0f, 1.0f, 1.0f);
	c[1] = glm::vec3(1.0f, 1.0f, 1.0f);  //white default
	c[2] = glm::vec3(1.0f, 1.0f, 1.0f);
}

// Rendering the triangle using OpenGL
void Triangle::RenderOpenGL(glm::mat4 &modelViewMatrix, glm::mat4 &projectionMatrix) 
{

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(modelViewMatrix));

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(projectionMatrix));

	glBegin(GL_TRIANGLES);
		glColor3f(c[0].x, c[0].y, c[0].z);
		glVertex3f(v[0].x, v[0].y, v[0].z);

		glColor3f(c[1].x, c[1].y, c[1].z);
		glVertex3f(v[1].x, v[1].y, v[1].z);   

		glColor3f(c[2].x, c[2].y, c[2].z);
		glVertex3f(v[2].x, v[2].y, v[2].z);
	glEnd();
}

bool Triangle::insideT(float x, float y, glm::vec4 p1, glm::vec4 p2, glm::vec4 p3) {
	
	float v0x = p1[0];
	float v0y = p1[1];
	float v1x = p2[0];
	float v1y = p2[1];
	float v2x = p3[0];
	float v2y = p3[1];
	
	if ( (-(v1y-v0y) * x) + ((v1x-v0x) * y) + ( v0x*v1y - v1x*v0y ) >= 0) {
		if ((-(v2y - v1y) * x) + ((v2x - v1x) * y) + (v1x*v2y - v2x * v1y) >= 0) {
			if ( ( (-(v0y-v2y)*x) + ((v0x-v2x)*y) + (v2x*v0y - v0x*v2y)) >= 0) {
				//sstd::cout << ":(*" << std::endl;
				return true;
			}
		}
	}
	return false;
}

// Render the triangle on CPU
void Triangle::RenderCPU(glm::mat4 &projection, glm::mat4 &modelViewMatrix, glm::mat4 &viewPort,float color[][1000][3], float depth[][1000],bool Zmode,float zMax, float zMin)
{
	int xMax = -999999999;
	int yMax = -999999999;
	int xMin = 999999999;
	int yMin = 999999999;
	Triangle Copy = *this;
	

	//first model (lookat) then perspective and finally viewport 

	glm::vec4 p1 = glm::vec4(v[0].x, v[0].y, v[0].z, 1.0f);
	glm::vec4 p2 = glm::vec4(v[1].x, v[1].y, v[1].z, 1.0f);
	glm::vec4 p3 = glm::vec4(v[2].x, v[2].y, v[2].z, 1.0f);
	
	p1 = modelViewMatrix * p1;
	p2 = modelViewMatrix * p2;
	p3 = modelViewMatrix * p3;

	p1 = projection * p1;
	p2 = projection * p2; //scale to correct pixel on screen 
	p3 = projection * p3;
	//std::cout << modelViewMatrix[0][3] << " " << modelViewMatrix[1][3] << " " << modelViewMatrix[2][3] << " " << modelViewMatrix[3][3] << " " << std::endl;
	
	//std::cout << p1[2] << std::endl;
	p1 = viewPort * p1;
	p2 = viewPort * p2; //scale to correct pixel on screen 
	p3 = viewPort * p3; 
	float z1 = p1[2];
	float z2 = p3[2];
	float z3 = p3[2];
	
	p1 /= p1[3];
	p2 /= p2[3];    //normalize clip space 
	p3 /= p3[3];
	
	/////////////////////////////////////////
	if (p1[0] >= xMax) {
		if(p1[0] <= 0){ xMax = 0; }
		else if (p1[0] >= 1000) {
			xMax = 1000;
		}
		else { xMax = ceil(p1[0]); }
	}
	if (p1[0] <= xMin) { 
		if (p1[0] > 1000) { xMin = 1000; }
		else if (p1[0] <= 0) {
			xMin = 0;
		}
		else { xMin = floor(p1[0]); }
	}                                   //P1 Bounding Box check 
	if (p1[1] >= yMax) {
		if (p1[1] <= 0) { yMax = 0; }
		else if (p1[1] >= 1000) {
			yMax = 1000;
		}
		else { yMax = ceil(p1[1]); }
	}
	if (p1[1] <= yMin) {
		if (p1[0] >= 1000) { yMin = 1000; }
		else if (p1[1] <= 0) {
			yMin = 0;
		}
		else { yMin = floor(p1[1]); }
	}

	////////////////////////////////////////
	if (p2[0] >= xMax) {
		if (p2[0] <= 0) { xMax = 0; }
		else if (p2[0] >= 1000) {
			xMax = 1000;
		}
		else { xMax = ceil(p2[0]); }
	}
	if (p2[0] <= xMin) {
		if (p2[0] >= 1000) { xMin = 1000; }
		else if (p2[0] <= 0) {
			xMin = 0;
		}
		else { xMin = floor(p2[0]); }
	}                                   //P2 Bounding Box check 
	if (p2[1] >= yMax) {
		if (p2[1] <= 0) { yMax = 0; }
		else if (p2[1] >= 1000) {
			yMax = 1000;
		}
		else { yMax = ceil(p2[1]); }
	}
	if (p2[1] <= yMin) {
		if (p2[0] >= 1000) { yMin = 1000; }
		else if (p2[1] <= 0) {
			yMin = 0;
		}
		else { yMin = floor(p2[1]); }
	}

	////////////////////////////////////////////////

	if (p3[0] >= xMax) {
		if (p3[0] <= 0) { xMax = 0; }
		else if (p3[0] >= 1000) {
			xMax = 1000;
		}
		else { xMax = ceil(p3[0]); }
	}
	if (p3[0] <= xMin) {
		if (p3[0] >= 1000) { xMin = 1000; }
		else if (p3[0] <= 0) {
			xMin = 0;
		}
		else { xMin = floor(p3[0]); }
	}                                   //P3 Bounding Box check 
	if (p3[1] >= yMax) {
		if (p3[1] <= 0) { yMax = 0; }
		else if (p3[1] >= 1000) {
			yMax = 1000;
		}
		else { yMax = ceil(p3[1]); }
	}
	if (p3[1] <= yMin) {
		if (p3[0] >= 1000) { yMin = 1000; }
		else if (p3[1] <= 0) {
			yMin = 0;
		}
		else { yMin = floor(p3[1]); }
	}
	/////////////////////////////////////////

	float z;
	for (int x = xMin; x < xMax; x++) {
		for (int y = yMin; y < yMax; y++) {
			if (insideT(x + 0.5, y + 0.5, p1, p2, p3)) {
				//color
				float alpha = ((-(x - p2[0])*(p3[1] - p2[1])) + (y - p2[1])*(p3[0] - p2[0])) / ((-(p1[0] - p2[0])*(p3[1] - p2[1])) + (p1[1] - p2[1])*(p3[0] - p2[0]));
				float beta = ((-(x - p3[0])*(p1[1] - p3[1])) + (y - p3[1])*(p1[0] - p3[0])) / ((-(p2[0] - p3[0])*(p1[1] - p3[1])) + (p2[1] - p3[1])*(p1[0] - p3[0]));
				float gamma = 1 - alpha - beta;
				z = alpha * z1 + beta * z2 + gamma * z3;
				
				if (z < depth[y][x]) {

					if (Zmode) {
						//std::cout << z << std::endl;
						
						//glm::vec3 zColor = glm::vec3(1.0f*(-(z-zMin)/(zMin-zMax)), 0.0f, 0.0f) ;
						glm::vec3 zColor = glm::vec3(1.0f/(pow(z,1.75)), 0.0f, 0.0f) ;
						//std::cout << z << std::endl;
						color[y][x][0] = zColor[0];
						color[y][x][1] = zColor[1];
						color[y][x][2] = zColor[2];
						depth[y][x] = z;
					}
					else {

						color[y][x][0] = alpha * c[0].x + beta * c[1].x + gamma * c[2].x;
						color[y][x][1] = alpha * c[0].y + beta * c[1].y + gamma * c[2].y;
						color[y][x][2] = alpha * c[0].z + beta * c[1].z + gamma * c[2].z;
						depth[y][x] = z;

					}
				}
			}
		}
	}
	memcpy(frameBuffer, renderedImage, sizeof(float) * WINDOW_HEIGHT * WINDOW_WIDTH * 3);
memcpy(frameBuffer, color, sizeof(color));
}