/************************************/
/************************************/
// Directly borrowed from John Keyser

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream> 
#include <string>
#include <vector>

#ifdef __APPLE__
	#define GenVertexArrays(A,B) glGenVertexArraysAPPLE(A,B)
	#define BindVertexArray(A) glBindVertexArrayAPPLE(A)
#else
	#define GenVertexArrays(A,B) glGenVertexArrays(A,B)
	#define BindVertexArray(A) glBindVertexArray(A)
#endif

// Thickness of the curve, in pixel.
#define LINE_WIDTH 3.0

// Screen setup
int windowWidth = 800;
int windowHeight = 600;

// OpenGL related variables
GLFWwindow *pWindow = NULL;
GLuint pid = 0, vao_curve = 0, vbo_curve = 0, vao_frame = 0, vbo_frame = 0;

// UI related variables
bool LB_pressed = false;
double x_old = 0, y_old = 0;
glm::mat4 rot, proj;

// Number of points on the curve. Don' change it by yourself. It will be automatically updated when calling sendAttributs().
int vec_size = 0;

// TODO: finish this function.
std::vector<glm::vec3> loadFile(const char* filename)
{
	std::ifstream In;
	std::string Name = "../DataFiles/";
	Name += filename;
	std::vector<glm::vec3> Data;
	In.open(Name);
	int Points; 
	In >> Points;
	for (int i = 0; i < Points; i++) {
		float x;
		float y;
		float z;
		In >> x;
		In >> y;
		In >> z;
		glm::vec3 A = glm::vec3(x, y, z);
		Data.push_back(A);
		}
	In.close();
	return Data;
}
glm::vec3 rEvaluate(float t,int depth, int InDepth, std::vector<glm::vec3> points,int Pindex) {
	glm::vec3 C= glm::vec3(0.0f,0.0f,0.0f);
	if (InDepth != depth) {
		//Traversing a Pascal Pyramid is like traversing a Binary tree with 
		//the inner nodes merged 
		InDepth += 1;
		//The Tree has to check 8 total leaves to 4 Points 
		//Or in general for n Points 2n leaves DFS
		//The index starts from the value of the Pyramid depth 
		Pindex -= 1;//Going to a left child reduces the index 
		C += (1-t)*rEvaluate(t, depth, InDepth , points,Pindex);//return left 
		Pindex += 1;//Going to a Right child increases the index
		C += (t)*rEvaluate(t, depth, InDepth, points,Pindex);//return Right 
		return	C;
	}
	else {//Leaf
		C = points[Pindex];
		return	C;
	}
}
// TODO: finish this function.
std::vector<glm::vec3> generatePoints(std::vector<glm::vec3> control_points)
{
	std::vector<glm::vec3> Out;
	int depth = control_points.size()-1;
	for (float i = 0.0; i < 1.0; i+=0.015) {//75 points
		glm::vec3 A = rEvaluate(i,depth,0, control_points, depth);
		Out.push_back(A);
	}
	return Out;
}

// Don't change the code following if unnecessary. 
// UI and Rotation related functions
glm::vec3 toVec3(double x, double y)
{
	glm::vec3 res;
	res.x = x / windowWidth * 2.0 - 1.0;
	res.y = 1.0 - y / windowHeight * 2.0;
	res.z = 1 - res.x * res.x - res.y * res.y;
	if (res.z <= 0)
	{
		res.z = 0;
		res = glm::normalize(res);
	}
	else
	{
		res.z = sqrt(res.z);
	}
	return res;
}

glm::mat4 getRotation(double x0, double y0, double x1, double y1)
{
	glm::vec3 p0 = toVec3(x0, y0), p1 = toVec3(x1, y1);
	glm::vec3 dir = glm::cross(p0, p1);
	float s = glm::length(dir);
	if (s < 1E-3)
	{
		return glm::mat4(1.0);
	}
	else
	{
		dir = glm::normalize(dir);
		if(glm::dot(p0, p1) >= 0)
			return glm::rotate(glm::mat4(1.0), asin(s) / 2.0f, dir);
		else
			return glm::rotate(glm::mat4(1.0), (3.14159265f - asin(s)) / 2.0f, dir);
	}
}

// Callback functions. 
void MouseButtonCallbackFunc(GLFWwindow* pwnd, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			glfwGetCursorPos(pwnd, &x_old, &y_old);
			LB_pressed = true;
		}
		else if (action == GLFW_RELEASE)
		{
			LB_pressed = false;
			double x_new = 0, y_new = 0;
			glfwGetCursorPos(pwnd, &x_new, &y_new);
			rot = getRotation(x_old, y_old, x_new, y_new) * rot;
		}
		glUseProgram(pid);
		glUniformMatrix4fv(glGetUniformLocation(pid, "mat"), 1, GL_FALSE, &((proj * rot)[0][0]));
	}
}

void CursorPosCallbackFunc(GLFWwindow* window, double xpos, double ypos)
{
	if (LB_pressed)
	{
		glUseProgram(pid);
		glUniformMatrix4fv(glGetUniformLocation(pid, "mat"), 1, GL_FALSE, &((proj * getRotation(x_old, y_old, xpos, ypos) * rot)[0][0]));
	}
}

void ScrollCallbackFunc(GLFWwindow* window, double xoffset, double yoffset)
{
	rot = glm::scale(rot, glm::vec3(pow(1.1, -yoffset)));
	glUseProgram(pid);
	glUniformMatrix4fv(glGetUniformLocation(pid, "mat"), 1, GL_FALSE, &((proj * rot)[0][0]));
}

void WindowSizeCallbackFunc(GLFWwindow* window, int width, int height)
{
	windowWidth = width;
	windowHeight = height;
	glViewport(0, 0, windowWidth, windowHeight);
	proj = glm::ortho(-10.0f * windowWidth / windowHeight, 10.0f * windowWidth / windowHeight, -10.0f, 10.0f, -1000.0f, 1000.0f);
	glUseProgram(pid);
	glUniformMatrix4fv(glGetUniformLocation(pid, "mat"), 1, GL_FALSE, &((proj * rot)[0][0]));
}

bool init()
{
	glfwInit();
	glfwSetTime(0.0);
	if (pWindow)
	{
		glfwDestroyWindow(pWindow);
	}
	pWindow = glfwCreateWindow(windowWidth, windowHeight, "Assignment 6 - Aldo Leon Marquez", NULL, NULL);
	glfwMakeContextCurrent(pWindow);
	glewExperimental = true;
	glewInit();
	glViewport(0, 0, windowWidth, windowHeight);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glfwSetMouseButtonCallback(pWindow, MouseButtonCallbackFunc);
	glfwSetCursorPosCallback(pWindow, CursorPosCallbackFunc);
	glfwSetScrollCallback(pWindow, ScrollCallbackFunc);
	glfwSetWindowSizeCallback(pWindow, WindowSizeCallbackFunc);
	glLineWidth(LINE_WIDTH);


	if (pid != 0)
	{
		std::cout << "This program is already created." << std::endl;
		return false;
	}
	GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
	GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* textvs =
		"#version 120\n"
		"uniform mat4 mat;\n"
		"attribute vec3 Pos;\n"
		"attribute vec3 Col;\n"
		"varying vec3 vCol;\n"
		"void main(){vCol=Col;gl_Position = mat * vec4(Pos, 1.0);}";
	glShaderSource(vert_shader, 1, &textvs, NULL);
	glCompileShader(vert_shader);
	const char* textfs =
		"#version 120\n"
		"varying vec3 vCol;\n"
		"void main(){gl_FragColor = vec4(vCol, 1.0);}";
	glShaderSource(frag_shader, 1, &textfs, NULL);
	glCompileShader(frag_shader);
	pid = glCreateProgram();
	glAttachShader(pid, vert_shader);
	glAttachShader(pid, frag_shader);
	glLinkProgram(pid);
	glDetachShader(pid, vert_shader);
	glDetachShader(pid, frag_shader);
	glUseProgram(pid);
	GenVertexArrays(1, &vao_curve);
	glGenBuffers(1, &vbo_curve);

	GenVertexArrays(1, &vao_frame);
	glGenBuffers(1, &vbo_frame);
	float data_buffer[] = {
			0.0,	0.0,	0.0,	1.0,	0.0,	0.0,
			1.0,	0.0,	0.0,	1.0,	0.0,	0.0,
			0.0,	0.0,	0.0,	0.0,	1.0,	0.0,
			0.0,	1.0,	0.0,	0.0,	1.0,	0.0,
			0.0,	0.0,	0.0,	0.0,	0.0,	1.0,
			0.0,	0.0,	1.0,	0.0,	0.0,	1.0,
			10.0,	10.0,	10.0,	0.7,	0.7,	0.7,
			-10.0,	10.0,	10.0,	0.7,	0.7,	0.7,
			10.0,	10.0,	-10.0,	0.7,	0.7,	0.7,
			-10.0,	10.0,	-10.0,	0.7,	0.7,	0.7,
			10.0,	-10.0,	10.0,	0.7,	0.7,	0.7,
			-10.0,	-10.0,	10.0,	0.7,	0.7,	0.7,
			10.0,	-10.0,	-10.0,	0.7,	0.7,	0.7,
			-10.0,	-10.0,	-10.0,	0.7,	0.7,	0.7,
			10.0,	10.0,	10.0,	0.7,	0.7,	0.7,
			10.0,	-10.0,	10.0,	0.7,	0.7,	0.7,
			-10.0,	10.0,	10.0,	0.7,	0.7,	0.7,
			-10.0,	-10.0,	10.0,	0.7,	0.7,	0.7,
			10.0,	10.0,	-10.0,	0.7,	0.7,	0.7,
			10.0,	-10.0,	-10.0,	0.7,	0.7,	0.7,
			-10.0,	10.0,	-10.0,	0.7,	0.7,	0.7,
			-10.0,	-10.0,	-10.0,	0.7,	0.7,	0.7,
			10.0,	10.0,	10.0,	0.7,	0.7,	0.7,
			10.0,	10.0,	-10.0,	0.7,	0.7,	0.7,
			10.0,	-10.0,	10.0,	0.7,	0.7,	0.7,
			10.0,	-10.0,	-10.0,	0.7,	0.7,	0.7,
			-10.0,	10.0,	10.0,	0.7,	0.7,	0.7,
			-10.0,	10.0,	-10.0,	0.7,	0.7,	0.7,
			-10.0,	-10.0,	10.0,	0.7,	0.7,	0.7,
			-10.0,	-10.0,	-10.0,	0.7,	0.7,	0.7
	};
	glUseProgram(pid);
	BindVertexArray(vao_frame);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_frame);

	glBufferData(GL_ARRAY_BUFFER, 180 * sizeof(float), data_buffer, GL_STATIC_DRAW);
	glEnableVertexAttribArray(glGetAttribLocation(pid, "Pos"));
	glEnableVertexAttribArray(glGetAttribLocation(pid, "Col"));

	glVertexAttribPointer(glGetAttribLocation(pid, "Pos"), 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), NULL);
	glVertexAttribPointer(glGetAttribLocation(pid, "Col"), 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	BindVertexArray(0);

	rot = glm::mat4(1.0);
	proj = glm::ortho(-10.0f * windowWidth / windowHeight, 10.0f * windowWidth / windowHeight, -10.0f, 10.0f, -1000.0f, 1000.0f);
	glUniformMatrix4fv(glGetUniformLocation(pid, "mat"), 1, GL_FALSE, &((proj * rot)[0][0]));
	return true;
}
// Don't change the code above if unnecessary.

// Send data to GPU. point_array is a vector of all the points on the curve.
// Each points has a position (x, y, z) and a color (r, g, b).
void sendAttributes(std::vector<glm::vec3> &point_array)
{
	vec_size = point_array.size();
	float *data_buffer = new float[6 * vec_size];
	for (int i = 0; i < point_array.size(); i++)
	{
		data_buffer[6 * i] = point_array[i].x;
		data_buffer[6 * i + 1] = point_array[i].y;
		data_buffer[6 * i + 2] = point_array[i].z;
		data_buffer[6 * i + 3] = 0.0f;
		data_buffer[6 * i + 4] = 0.0f;
		data_buffer[6 * i + 5] = 0.0f;
	}
	glUseProgram(pid);
	BindVertexArray(vao_curve);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_curve);

	glBufferData(GL_ARRAY_BUFFER, vec_size * 6 * sizeof(float), data_buffer, GL_STATIC_DRAW);
	glEnableVertexAttribArray(glGetAttribLocation(pid, "Pos"));
	glEnableVertexAttribArray(glGetAttribLocation(pid, "Col"));

	glVertexAttribPointer(glGetAttribLocation(pid, "Pos"), 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), NULL);
	glVertexAttribPointer(glGetAttribLocation(pid, "Col"), 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	BindVertexArray(0);
}

void display(void)
{
	glUseProgram(pid);
	// Draw the curve.
	BindVertexArray(vao_curve);
	glDrawArrays(GL_LINE_STRIP, 0, vec_size);
	// Draw the frame.
	BindVertexArray(vao_frame);
	glDrawArrays(GL_LINES, 0, 30);
	BindVertexArray(0);
}

int main(int argc, char** argv)
{
	init();
	if (argc != 2)
	{
		std::cout << "Usage: " << argv[0] << " data_file" << std::endl;\
		// An example of how to draw a curve.
		std::vector<glm::vec3> point_array;
		for (float i = -10.0; i <= 10.0; i += 0.5)
			point_array.push_back(glm::vec3(5.0 * cos(i), 5.0 * sin(i), i));
		sendAttributes(point_array);
	}
	else
	{
		std::vector<glm::vec3> point_array = generatePoints(loadFile(argv[1]));
		sendAttributes(point_array);
	}
	while (!glfwWindowShouldClose(pWindow)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		display();
		glfwSwapBuffers(pWindow);
		glfwPollEvents();
	}
	return 0;
}