#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "Camera.h"
#include "Scene.h"
#include "Shape.h"
#include "Plane.h"
#include "Sphere.h"



#define WINDOW_HEIGHT 1080
#define WINDOW_WIDTH 1920
float frameBuffer[WINDOW_HEIGHT][WINDOW_WIDTH][3];
GLFWwindow *window;

std::vector<Shape*> shapes;
std::vector<Light*> lights;




void ClearFrameBuffer()
{
	memset(&frameBuffer, 0, WINDOW_HEIGHT * WINDOW_WIDTH * 3 * sizeof(float));
}

void Display()
{	
	glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_FLOAT, frameBuffer);
}

void Init()
{

	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Assignment5 - Aldo Leon Marquez", NULL, NULL);
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	ClearFrameBuffer();
	Scene scene = Scene();
	//Add figures 

	Shape* S1;
	Sphere One;
	One.R = 0.3f;
	One.s = 100.0f;
	One.Position = glm::vec3(-1.0f, -0.7f, 3.0f);
	One.ka = glm::vec3(0.1f, 0.1f, 0.1f);
	One.kd = glm::vec3(0.2f, 1.0f, 0.2f);
	One.ks = glm::vec3(1.0f, 1.0f, 1.0f);
	One.km = glm::vec3(0.0f, 0.0f, 0.0f);
	S1 = &One;
	scene.shapes.push_back(S1);

	
	Shape* S2;
	Sphere Two;
	Two.R = 0.5f;
	Two.s = 10.0f;
	Two.Position = glm::vec3(1.0f, -0.5f, 3.0f);
	Two.ka = glm::vec3(0.1f, 0.1f, 0.1f);
	Two.kd = glm::vec3(0.0f, 0.0f, 1.0f);
	Two.ks = glm::vec3(1.0f, 1.0f, 1.0f);
	Two.km = glm::vec3(0.0f, 0.0f, 0.0f);
	S2 = &Two;
	scene.shapes.push_back(S2);
	
	Shape* S3;
	Sphere Three;
	Three.R = 1.0f;
	Three.s = 0.0f;
	Three.Position = glm::vec3(-1.0f, 0.0f, 0.0f);
	Three.ka = glm::vec3(0.0f, 0.0f, 0.0f);
	Three.kd = glm::vec3(0.0f, 0.0f, 0.0f);
	Three.ks = glm::vec3(0.0f, 0.0f, 0.0f);
	Three.km = glm::vec3(1.0f, 1.0f, 1.0f);
	S3 = &Three;
	scene.shapes.push_back(S3);
	
	Shape* S4;
	Sphere Four;
	Four.R = 1.0f;
	Four.s = 0.0f;
	Four.Position = glm::vec3(1.0f, 0.0f, -1.0f);
	Four.ka = glm::vec3(0.0f, 0.0f, 0.0f);
	Four.kd = glm::vec3(0.0f, 0.0f, 0.0f);
	Four.ks = glm::vec3(0.0f, 0.0f, 0.0f);
	Four.km = glm::vec3(0.8f, 0.8f, 0.8f);
	S4 = &Four;
	scene.shapes.push_back(S4);

	Shape* P1;
	Plane Pone;
	Pone.Center = glm::vec3(0.0f, -1.0f, 0.0f);
	Pone.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
	Pone.ka = glm::vec3(0.1f, 0.1f, 0.1f);
	Pone.kd = glm::vec3(1.0f, 1.0f, 1.0f);
	Pone.ks = glm::vec3(0.0f, 0.0f, 0.0f);
	Pone.km = glm::vec3(0.0f, 0.0f, 0.0f);
	Pone.s = 0.0f;
	P1 = &Pone;
	scene.shapes.push_back(P1);
	
	Shape* P2;
	Plane Ptwo;
	Ptwo.Center = glm::vec3(0.0f, 0.0f, -3.0f);
	Ptwo.Normal = glm::vec3(0.0f, 0.0f, 1.0f);
	Ptwo.ka = glm::vec3(0.1f, 0.1f, 0.1f);
	Ptwo.kd = glm::vec3(1.0f, 1.0f, 1.0f);
	Ptwo.ks = glm::vec3(0.0f, 0.0f, 0.0f);
	Ptwo.km = glm::vec3(0.0f, 0.0f, 0.0f);
	Ptwo.s = 0.0f;
	P2 = &Ptwo;
	scene.shapes.push_back(P2);
	
	
	Light L1;
	L1.Position = glm::vec3(0.0f, 3.0f, -2.0f);
	L1.Color = glm::vec3(0.2f, 0.2f, 0.2f);
	scene.lights.push_back(&L1);
	
	
	Light L2;
	L2.Position = glm::vec3(-2.0f, 1.0f, 4.0f);
	L2.Color = glm::vec3(0.5f, 0.5f, 0.5f);
	scene.lights.push_back(&L2);
	
	Camera camera = Camera(WINDOW_WIDTH, WINDOW_HEIGHT);
	
	camera.TakePicture(&scene,frameBuffer);
	//CImg  to dumb buffer to a file 
	//Instance then save the buffer to it 
	//Implement area shadows as a class 
}

int main()
{	
	Init();
	while ( glfwWindowShouldClose(window) == 0) 
	{
		glClear(GL_COLOR_BUFFER_BIT);
		Display();
		glFlush();
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwTerminate();
	return 0;
}