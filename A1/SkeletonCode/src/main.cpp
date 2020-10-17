
#define GLEW_STATIC
#define _USE_MATH_DEFINES
#include <limits>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>


#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 1080

float frameBuffer[WINDOW_HEIGHT][WINDOW_WIDTH][3];
bool mask[WINDOW_HEIGHT][WINDOW_WIDTH];
int Mask[1280][1080] = {0};
char Brush = 's';
bool Random = false;

GLFWwindow *window;

int s = 4; // default 4,
// Color structure. Can be used to define the brush and background color.
struct color { float r, g, b; };
color A{1,1,1};


// A function clamping the input values to the lower and higher bounds
#define CLAMP(in, low, high) ((in) < (low) ? (low) : ((in) > (high) ? (high) : in))

// Set a particular pixel of the frameBuffer to the provided color
void SetFrameBufferPixel(int x, int y, struct color lc)
{
	// Origin is upper left corner. 
	// Changes the origin from the lower-left corner to the upper-left corner
	y = WINDOW_HEIGHT - 1 - y;

	x = CLAMP(x, 0, WINDOW_WIDTH - 1);
	y = CLAMP(y, 0, WINDOW_HEIGHT - 1);

	frameBuffer[y][x][0] = lc.r;
	frameBuffer[y][x][1] = lc.g;
	frameBuffer[y][x][2] = lc.b;
}

void ClearFrameBuffer()
{
	memset(frameBuffer, 0.0f, sizeof(float) * WINDOW_WIDTH * WINDOW_HEIGHT * 3);
}

// Display frameBuffer on screen
void Display()
{	
	glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_FLOAT, frameBuffer);
}

// Mouse position callback function
void CursorPositionCallback(GLFWwindow* lWindow, double xpos, double ypos)
{
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS && Brush == 's')
	{
		
		if (s <= 0) {
			s = 1;
		}
		for (int i = xpos - s; i < xpos + s; i++) {
			for (int j = ypos - s; j < ypos + s; j++) {
				if (Random == true) {
					int a = rand() % s;
					if (a == 0) {
						SetFrameBufferPixel(i, j, A);
						Mask[i][j] = 1;
					}
				}
				else {
					SetFrameBufferPixel(i, j, A);
					Mask[i][j] = 1;
				}
			}
		}
		//std::cout << "Mouse position is: x - " << xpos << ", y - " << ypos << std::endl;
		
	}
	
	else if (state == GLFW_PRESS && Brush == 'c') {
		int Height = s;
		double Interval = (180 * M_PI / 180) / (2 * s + 1);
		double angle = M_PI;
		for (int i = xpos - s; i <= xpos + s; i++) {
			double b = floor(abs(Height*pow(sin(angle),.5)));
			angle -= Interval;
			//std::cout << angle << "     "<<(i-xpos) << "   " << Interval<< std::endl;
			//std::cout << b<< std::endl;
			//P = P + (xpos-i);
			int P = b;
			if (Random == true) {
				for (int j = ypos - P; j < ypos + P; j++) {
					int a = rand() % s;
					if (a == 0) {

						SetFrameBufferPixel(i, j, A);
						Mask[i][j] = 1;
					}
				}
			}
			else {
				for (int j = ypos - P; j < ypos + P; j++) {
					SetFrameBufferPixel(i, j, A);
					Mask[i][j] = 1;
				}
			}
		}
	}
}

// Mouse callback function
void MouseCallback(GLFWwindow* lWindow, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) 
	{
		std::cout << "Mouse left button is pressed." << std::endl;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		for (int i = 0; i < 1280; i++) {
			for (int j = 0; j < 1080; j++) {
				Mask[i][j] = 0;
			}
		}
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		ClearFrameBuffer();
		glClear(GL_COLOR_BUFFER_BIT);
	}
}
// You can use "switch" or "if" to compare key to a specific character.
// for example,
// if (key == '0')
//     DO SOMETHING

// Keyboard callback function
void CharacterCallback(GLFWwindow* lWindow, unsigned int key)
{
	std::cout << "Key " << (char)key << " is pressed." << std::endl;
	if (key == '+') {
		if (s <= 64) {
			s = s*2;
		}
		else if (s <= 0) {
			s = 1;
		}
	}
	else if (key == '-') {
		if (s >= 1) {
			s = s / 2;
		}
		else if (s <= 0) {
			s = 1;
		}
	}
	else if (key == '0') {
		A.r = 0;
		A.g = 0;
		A.b = 0;
	}
	else if (key == '1') {
		A.r = 1;
		A.g = 0;
		A.b = 0;
	}
	else if (key == '2') {
		A.r = 0;
		A.g = 1;
		A.b = 0;
	}
	else if (key == '3') {
		A.r = 1;
		A.g = 1;
		A.b = 0;
	}
	else if (key == '4') {
		A.r = 0;
		A.g = 0;
		A.b = 1;
	}
	else if (key == '5') {
		A.r = 1;
		A.g = 0;
		A.b = 1;		
	}
	else if (key == '6') {
		A.r = 0;
		A.g = 1;
		A.b = 1;
	}
	else if (key == '7') {
		A.r = 1;
		A.g = 1;
		A.b = 1;
	}
	else if (key == ')') {
		A.r = 0;
		A.g = 0;
		A.b = 0;
		for (int y = 0; y < WINDOW_WIDTH; y++) {
			for (int x = 0; x < WINDOW_HEIGHT; x++) {
				if (Mask[y][x] != 1) {
					SetFrameBufferPixel(y, x, A);
				}
			}
		}
	}
	else if (key == '!') {
		A.r = 1;
		A.g = 0;
		A.b = 0;
		for (int y = 0; y < WINDOW_WIDTH; y++) {
			for (int x = 0; x < WINDOW_HEIGHT; x++) {
				if (Mask[y][x] != 1) {
					SetFrameBufferPixel(y, x, A);
				}
			}
		}
	}
	else if (key == '@') {
		A.r = 0;
		A.g = 1;
		A.b = 0;
		for (int y = 0; y < WINDOW_WIDTH; y++) {
			for (int x = 0; x < WINDOW_HEIGHT; x++) {
				if (Mask[y][x] != 1) {
					SetFrameBufferPixel(y, x, A);
				}
			}
		}
	}
	else if (key == '#') {
		A.r = 1;
		A.g = 1;
		A.b = 0;
		for (int y = 0; y < WINDOW_WIDTH; y++) {
			for (int x = 0; x < WINDOW_HEIGHT; x++) {
				if (Mask[y][x] != 1) {
					SetFrameBufferPixel(y, x, A);
				}
			}
		}
	}
	else if (key == '$') {
		A.r = 0;
		A.g = 0;
		A.b = 1;
		for (int y = 0; y < WINDOW_WIDTH; y++) {
			for (int x = 0; x < WINDOW_HEIGHT; x++) {
				if (Mask[y][x] != 1) {
					SetFrameBufferPixel(y, x, A);
				}
			}
		}
	}
	else if (key == '%') {
	A.r = 1;
	A.g = 0;
	A.b = 1;
	for (int y = 0; y < WINDOW_WIDTH; y++) {
			for (int x = 0; x < WINDOW_HEIGHT; x++) {
				if (Mask[y][x] != 1) {
					SetFrameBufferPixel(y, x, A);
				}
			}
		}
	}
	else if (key == '^') {
	A.r = 0;
	A.g = 1;
	A.b = 1;
		for (int y = 0; y < WINDOW_WIDTH; y++) {
			for (int x = 0; x < WINDOW_HEIGHT; x++) {
				if (Mask[y][x] != 1) {
					SetFrameBufferPixel(y, x, A);
				}
			}
		}
	}
	else if (key == '&') {
	A.r = 1;
	A.g = 1;
	A.b = 1;
		for (int y = 0; y < WINDOW_WIDTH; y++) {
			for (int x = 0; x < WINDOW_HEIGHT; x++) {
				if (Mask[y][x] != 1) {
					SetFrameBufferPixel(y, x, A);
				}
			}
		}
	}
	else if (key == 'b') {
		if (Brush == 's') {
			std::cout << "wtf Man "<<Brush<<std::endl;
			Brush = 'c';
			}
		else if (Brush == 'c') {
			Brush = 's';
		}
	}
	else if (key == 'r'){
		if (Random == false) {
			std::cout << "wtf MaN :) " << std::endl;
			Random = true;
			}
		else if (Random == true) {
			Random = false;
		}
	}

}

void Init()
{
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Assignment 1 - Aldo Leon Marquez", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetMouseButtonCallback(window, MouseCallback);
	glfwSetCursorPosCallback(window, CursorPositionCallback);
	glfwSetCharCallback(window, CharacterCallback);
	glewExperimental = GL_TRUE;
	glewInit();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	ClearFrameBuffer();
}


int main()
{	
	Init();
	while (glfwWindowShouldClose(window) == 0)
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