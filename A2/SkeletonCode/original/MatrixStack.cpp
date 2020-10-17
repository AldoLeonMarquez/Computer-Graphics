
#include "MatrixStack.h"

#include <stdio.h>
#include <cassert>
#include <vector>

#include <iostream>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

MatrixStack::MatrixStack()
{
	mstack = make_shared< stack<glm::mat4> >();
	mstack->push(glm::mat4(1.0));
}

MatrixStack::~MatrixStack()
{
}

void MatrixStack::pushMatrix()
{
	const glm::mat4 &top = mstack->top();
	mstack->push(top);
	assert(mstack->size() < 100);
}

void MatrixStack::popMatrix()
{
	assert(!mstack->empty());
	mstack->pop();
	// There should always be one matrix left.
	assert(!mstack->empty());
}

void MatrixStack::loadIdentity()
{
	glm::mat4 &top = mstack->top();
	top = glm::mat4(1.0);
}

void MatrixStack::translate(const glm::vec3 &t) 
{
	glm::mat4 translationMatrix(1.0f); //identity matrix
	translationMatrix[3][0] = t[0];   //+-x
	translationMatrix[3][1] = t[1];   //+-y 
	translationMatrix[3][2] = t[2];   //+-z

	// Need to comment out the following line and write your own version    //done
	//translationMatrix = glm::translate(glm::mat4(1.0f), t);
	multMatrix(translationMatrix);
}

void MatrixStack::scale(const glm::vec3 &s)
{
	glm::mat4 scaleMatrix(1.0f);
	scaleMatrix[0][0] = s[0];   //+-x
	scaleMatrix[1][1] = s[1];   //+-y       //done
	scaleMatrix[2][2] = s[2];   //+-z

	// Need to comment out the following line and write your own version
	//scaleMatrix = glm::scale(glm::mat4(1.0f), s);

	multMatrix(scaleMatrix);
}

void MatrixStack::rotateX(float angle)
{
	//column x row
	glm::mat4 rotationMatrix(1.0f);
	rotationMatrix[1][1] = glm::cos(angle);
	rotationMatrix[2][1] = -(glm::sin(angle));       //done
	rotationMatrix[1][2] = glm::sin(angle);
	rotationMatrix[2][2] = glm::cos(angle);
	// Need to comment out the following line and write your own version
	//rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));

	multMatrix(rotationMatrix);
}

void MatrixStack::rotateY(float angle)
{
	//column x row
	glm::mat4 rotationMatrix(1.0f);
	rotationMatrix[0][0] = glm::cos(angle);
	rotationMatrix[2][0] = glm::sin(angle);         //done
	rotationMatrix[0][2] = -(glm::sin(angle));
	rotationMatrix[2][2] = glm::cos(angle);

	// Need to comment out the following line and write your own version
	//rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));

	multMatrix(rotationMatrix);
}

void MatrixStack::rotateZ(float angle)
{
	//column x row
	glm::mat4 rotationMatrix(1.0f);
	rotationMatrix[0][0] = glm::cos(angle); 
	rotationMatrix[1][0] = -(glm::sin(angle));     //done
	rotationMatrix[0][1] = glm::sin(angle);
	rotationMatrix[1][1] = glm::cos(angle);

	// Need to comment out the following line and write your own version
	//rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));

	multMatrix(rotationMatrix);
}

void MatrixStack::multMatrix(glm::mat4 &matrix)
{
	glm::mat4 &top = mstack->top();

	// Need to comment out the following line and write your own version
	//column x row
	glm::mat4 N = top;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			float temp = 0.0f;
			for (int k = 0; k < 4; k++) {            //done
				temp += top[k][j] * matrix[i][k];    //Note to self: Works, dont touch
			}
			N[i][j] = temp;
		}
	}
	top = N;
	
	//top *= matrix;

	// Getting a pointer to the glm::mat4 matrix
	float* topArray = glm::value_ptr(top);
	float* matrixArray = glm::value_ptr(matrix);
}

void MatrixStack::Perspective(float fovy, float aspect, float near, float far)
{
	//column x row
	glm::mat4 projectionMatrix(0.0f);
	float d = 1/(glm::tan(fovy / 2));
	projectionMatrix[0][0] = d / (aspect);
	projectionMatrix[1][1] = d;
	projectionMatrix[2][2] = -((far + near) / (far - near));     //done
	projectionMatrix[3][2] = -((2*far*near) / (far - near));
	projectionMatrix[2][3] = -1;

	// Need to comment out the following line and write your own version
	//projectionMatrix = glm::perspective(fovy, aspect, near, far);

	multMatrix(projectionMatrix);
}

void MatrixStack::LookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up)
{
	//column x row
	glm::mat4 viewMatrix(1.0f);
	glm::vec3 forward = (eye - center) / glm::length(eye - center);
	glm::vec3 left = glm::cross(up ,forward);
	glm::vec3 NewUp = glm::cross(forward, left);
	// Need to comment out the following line and write your own version
	//viewMatrix = glm::lookAt(eye, center, up);
	

	viewMatrix[0][0] = left[0] ;
	viewMatrix[1][0] = left[1];
	viewMatrix[2][0] = left[2];
	viewMatrix[0][1] = NewUp[0];
	viewMatrix[1][1] = NewUp[1];
	viewMatrix[2][1] = NewUp[2];
	viewMatrix[0][2] = forward[0];
	viewMatrix[1][2] = forward[1];
	viewMatrix[2][2] = forward[2];

	glm::mat4 viewTranslate(1.0f);
	viewTranslate[3][0] = -eye[0];
	viewTranslate[3][1] = -eye[1];
	viewTranslate[3][2] = -eye[2];

	viewMatrix *= viewTranslate;

	multMatrix(viewMatrix);
}


void MatrixStack::translate(float x, float y, float z)
{
	translate(glm::vec3(x, y, z));
}

void MatrixStack::scale(float x, float y, float z)
{
	scale(glm::vec3(x, y, z));
}

void MatrixStack::scale(float s)
{
	scale(glm::vec3(s, s, s));
}

glm::mat4 &MatrixStack::topMatrix()
{
	return mstack->top();
}

void MatrixStack::print(const glm::mat4 &mat, const char *name)
{
	if(name) {
		printf("%s = [\n", name);
	}
	for(int i = 0; i < 4; ++i) {
		for(int j = 0; j < 4; ++j) {
			// mat[j] returns the jth column
			printf("%- 5.2f ", mat[j][i]);
		}
		printf("\n");
	}
	if(name) {
		printf("];");
	}
	printf("\n");
}

void MatrixStack::print(const char *name) const
{
	print(mstack->top(), name);
}
