
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>
#include "MatrixStack.h"
#include "Program.h"


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800


float a = 0.0; //test purpose 

GLFWwindow *window;
double currentXpos, currentYpos;
glm::vec3 eye(0.0f, 0.0f, 8.0f);
glm::vec3 center(0.0f, 0.0f, 0.0f);
glm::vec3 up(0.0f, 1.0f, 0.0f);

Program program;
MatrixStack modelViewProjectionMatrix;

// Draw cube on screen
void DrawCube(glm::mat4& modelViewProjectionMatrix)
{
	program.SendUniformData(modelViewProjectionMatrix, "mvp");
glDrawArrays(GL_TRIANGLES, 0, 36);
}

class RobotElements
{
private:
	
public:
	RobotElements* Parent;
	std::vector<RobotElements*> Children;

	glm::vec3 JointParent; //Joint trasnlation from parent Join 
	glm::vec3 JointAngles = glm::vec3(0.0f,0.0f,0.0f); //Angle of the joint 
	glm::vec3 JointTranslate;  //Joint with respect of center of part 
	glm::vec3 Scale;

	RobotElements() {
		Parent = NULL;
	}
	~RobotElements() {}
	void RDraw(RobotElements* Current) {

		modelViewProjectionMatrix.pushMatrix();
		
			modelViewProjectionMatrix.translate(Current->JointParent);
			modelViewProjectionMatrix.rotateX(glm::radians(Current->JointAngles[0]));
			modelViewProjectionMatrix.rotateY(glm::radians(Current->JointAngles[1]));
			modelViewProjectionMatrix.rotateZ(glm::radians(Current->JointAngles[2]));   //Note to self: Works dont touch 
	
			modelViewProjectionMatrix.pushMatrix();

				
				modelViewProjectionMatrix.translate(Current->JointTranslate);
				modelViewProjectionMatrix.scale(Current->Scale);
				DrawCube(modelViewProjectionMatrix.topMatrix());

			modelViewProjectionMatrix.popMatrix();
		for (int i = 0; i < Current->Children.size(); i++) {
			modelViewProjectionMatrix.pushMatrix();
			this->RDraw(Current->Children[i]);
			modelViewProjectionMatrix.popMatrix();
		}
	
		modelViewProjectionMatrix.popMatrix();
	}
};


RobotElements* Selector[10]; //Hacky solution to change the part 
int CurrPiece = 0; //Starts at Torso


void ConstructRobot()
{
	RobotElements* Torso = new RobotElements();   
	Selector[0] = Torso;
	//root
	//The Joint of the Torso is the center of it, i.e the origin and center of the component itself 
	//Joint and component same for torso 
	Torso->JointTranslate= glm::vec3(0.0f,0.0f,0.0f);
	//Torso->JointAngles = glm::vec3(0.0f, 0.0f, 0.0f);
	Torso->Scale = glm::vec3(.75f,1.5f,.75f);
	//The torso has the origin as the joint parent
	Torso->JointParent = glm::vec3(0.0f, 1.3f, 0.0f);  
	
	RobotElements* Head = new RobotElements();
	Selector[1] = Head;
	Torso->Children.push_back(Head);
	Head->Parent = Torso;
	Head->JointParent = glm::vec3(0.0f,1.5f,0.0f);
	Head->JointAngles = glm::vec3(0.0f, 0.0f, 0.0f);
	Head->Scale = glm::vec3(0.5f, 0.5f, 0.5f);
	Head->JointTranslate = glm::vec3(0.0f, 0.5f, 0.0f);
	
	RobotElements* UpperLeftArm = new RobotElements();
	Selector[2] = UpperLeftArm;
	Torso->Children.push_back(UpperLeftArm);
	UpperLeftArm->Parent = Torso;
	UpperLeftArm->JointParent = glm::vec3(-0.75f,1.5f,0.0f);
	UpperLeftArm->JointAngles = glm::vec3(0.0f,0.0f,0.0f);
	UpperLeftArm->Scale = glm::vec3(1.0f,0.25,0.25f);
	UpperLeftArm->JointTranslate = glm::vec3(-1.0f,-0.25f,0.0f);

	RobotElements* LowerLeftArm = new RobotElements();
	Selector[3] = LowerLeftArm;
	UpperLeftArm->Children.push_back(LowerLeftArm);
	LowerLeftArm->Parent = UpperLeftArm;
	LowerLeftArm->JointParent = glm::vec3(-2.0f,-0.25f,0.25f);
	LowerLeftArm->JointAngles = glm::vec3(0.0f,0.0f,0.0f);
	LowerLeftArm ->Scale = glm::vec3(0.8f,0.15f,0.25f);
	LowerLeftArm->JointTranslate = glm::vec3(-0.8f,0.0f,-0.25f);

	RobotElements* UpperRightArm = new RobotElements();
	Selector[4] = UpperRightArm;
	Torso->Children.push_back(UpperRightArm);
	UpperRightArm->Parent = Torso;
	UpperRightArm->JointParent = glm::vec3(0.75f, 1.5f, 0.0f);
	UpperRightArm->JointAngles = glm::vec3(0.0f,0.0f,0.0f);
	UpperRightArm->Scale = glm::vec3(1.0f, 0.25, 0.25f);
	UpperRightArm->JointTranslate = glm::vec3(1.0f, -0.25f, 0.0f);

	RobotElements* LowerRightArm = new RobotElements();
	Selector[5] = LowerRightArm;
	UpperRightArm->Children.push_back(LowerRightArm);
	LowerRightArm->Parent = UpperRightArm;
	LowerRightArm->JointParent = glm::vec3(2.0f, -0.25f, 0.25f);
	LowerRightArm->JointAngles = glm::vec3(0.0f,0.0f,0.0f);
	LowerRightArm->Scale = glm::vec3(0.8f, 0.15f, 0.25f);
	LowerRightArm->JointTranslate = glm::vec3(0.8f, 0.0f, -0.25f);
	
	RobotElements* UpperLeftLeg = new RobotElements();
	Selector[6] = UpperLeftLeg;
	Torso->Children.push_back(UpperLeftLeg);
	UpperLeftLeg->Parent = Torso;
	UpperLeftLeg->JointParent = glm::vec3(-0.5f,-1.5f,-0.25f);
	UpperLeftLeg->JointAngles = glm::vec3(0.0f,0.0f,0.0f);
	UpperLeftLeg->Scale = glm::vec3(0.25f,1.15f,0.25f);
	UpperLeftLeg->JointTranslate = glm::vec3(0.0f,-1.15f,+0.25f);

	RobotElements* LowerLeftLeg = new RobotElements();
	Selector[7] = LowerLeftLeg;
	UpperLeftLeg->Children.push_back(LowerLeftLeg);
	LowerLeftLeg->Parent = UpperLeftLeg;
	LowerLeftLeg->JointParent = glm::vec3(0.0f,-2.3f,0.0f);
	LowerLeftLeg->JointAngles = glm::vec3(0.0f,0.0f,0.0f);
	LowerLeftLeg->Scale = glm::vec3(0.20f,1.0f,0.25f);
	LowerLeftLeg->JointTranslate = glm::vec3(0.0f,-1.0f,0.25f);

	RobotElements* UpperRightLeg = new RobotElements();
	Selector[8] = UpperRightLeg;
	Torso->Children.push_back(UpperRightLeg);
	UpperRightLeg->Parent = Torso;
	UpperRightLeg->JointParent = glm::vec3(0.5f, -1.5f, -0.25f);
	UpperRightLeg->JointAngles = glm::vec3(0.0f,0.0f,0.0f);
	UpperRightLeg->Scale = glm::vec3(0.25f, 1.15f, 0.25f);
	UpperRightLeg->JointTranslate = glm::vec3(0.0f, -1.15f, +0.25f);

	RobotElements* LowerRightLeg = new RobotElements();
	Selector[9] = LowerRightLeg;
	UpperRightLeg->Children.push_back(LowerRightLeg);
	LowerRightLeg->Parent = UpperRightLeg;
	LowerRightLeg->JointParent = glm::vec3(0.0f, -2.3f, 0.0f);
	LowerRightLeg->JointAngles = glm::vec3(0.0f,0.0f,0.0f);
	LowerRightLeg->Scale = glm::vec3(0.20f, 1.0f, 0.25f);
	LowerRightLeg->JointTranslate = glm::vec3(0.0f, -1.0f, 0.25f);
	
}

void Display()
{

	program.Bind();
	
	modelViewProjectionMatrix.loadIdentity();
	modelViewProjectionMatrix.pushMatrix();
	// Setting the view and Projection matrices
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	modelViewProjectionMatrix.Perspective(glm::radians(60.0f), float(width) / float(height), 0.1f, 100.0f);
	
	modelViewProjectionMatrix.LookAt(eye, center, up);

	Selector[0]->RDraw(Selector[0]); //Always draw starting from the Torso
	
	
	modelViewProjectionMatrix.popMatrix();

	program.Unbind();

}


float phi, theta = 0.0; // Spherical coordinates  angles 

// Mouse callback function
void MouseCallback(GLFWwindow* lWindow, int button, int action, int mods)
{



}
//Mouse Scroll Callback Function
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{


	if (yoffset == -1) {
		eye *= 0.9;
	}
	if (yoffset == 1) {
		eye *= 1.1;
	}
}  

int Oldxpos=0;
int Oldypos=0;
float DiffEandC = abs(eye[2] - center[2]);

// Mouse position callback function
void CursorPositionCallback(GLFWwindow* lWindow, double xpos, double ypos)
{
	 DiffEandC = abs(eye[2] - center[2]);
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	
	 if(state == GLFW_PRESS && xpos >0 && ypos >0 && xpos<801 && ypos <801) {	
	 
		 if (Oldxpos - xpos > 0 ) { //Left
			 theta--;
			// std::cout << "Left  " <<theta<<std::endl;
		 }
		 if (Oldypos - ypos > 0) {//Up
			 phi++;
			 //std::cout << "UP" << std::endl;
		 }
		 if (Oldxpos - xpos < 0) {//Right
			 theta++;
		 }
		 if (Oldypos - ypos < 0) { //down
			 phi--;
		 }
		 if (phi > 180.0) {
			 phi = 0;
		 }
		 if (phi < -180.0) {
			 phi = 179;
		 }
		 if (theta > 360.0) {
			 theta = 0;
		 }
		 if (theta < -360.0) {
			 theta  = 359;
		 }
		 Oldxpos = xpos;
		 Oldypos = ypos;
		 //Rodrigues Rotation
		 glm::vec3 EandC = (center -eye);
		 glm::vec3 Right = cross(EandC, up);
		 glm::mat4 View(1.0f);
		 glm::mat4 View2(1.0f);
		 glm::vec4 NewUp(up[0], up[1], up[2], 1.0f); //Vector with w included, necessary to apply trasnform to;
		 View = glm::rotate(glm::mat4(1.0f),glm::radians(theta), up); //Get NewUp position
		 View2 = glm::rotate(glm::mat4(1.0f),glm::radians(phi), Right); //Get newRight position
		 NewUp = View2* NewUp;
		 up[0] = NewUp[0];
		 up[1] = NewUp[1];
		 up[2] = NewUp[2];
		 glm::vec4 NewEye(eye[0], eye[1], eye[2],1.0f); //Vector with w included, necessary to apply trasnform to;
		 NewEye =  View* NewEye;
		 NewEye = View2 * NewEye; //Apply transforms to the Temporal Eye 
		 eye[0] = NewEye[0];
		 eye[1] = NewEye[1];
		 eye[2] = NewEye[2];
		 
	}

	 int state2 = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
	 if (state2 == GLFW_PRESS && xpos > 0 && ypos > 0 && xpos < 801 && ypos < 801) {
		 if (Oldxpos - xpos > 0) { //Left
			 eye[0] += .05;
			 center[0] += .05;
		 }
		 if (Oldypos - ypos > 0) {//Up
			 eye[1] -= .05;
			 center[1] -= .1;
		 }
		 if (Oldxpos - xpos < 0) {//Right
			 eye[0] -= .05;
			 center[0] -= .05;
		 }
		 if (Oldypos - ypos < 0) { //down
			 eye[1] += .05;
			 center[1] += .05;
		 }
	 }
	
	 
	 phi = 0.0f; //Extremely important to reset angles for camara rotation
	 theta = 0.0f;
	 Oldxpos = xpos;
	 Oldypos = ypos;
}


// Keyboard character callback function
void CharacterCallback(GLFWwindow* lWindow, unsigned int key)
{
	std::cout << "Key " << (char)key << " is pressed." << std::endl;
	if (key == 'a') { a++; }

	if (key == '.') {
		Selector[CurrPiece]->Scale -= glm::vec3(0.08f, 0.08f, 0.08f); //Not proud 
		CurrPiece++;
		
		if (CurrPiece > 9) {
			CurrPiece = 0;
		}
		Selector[CurrPiece]->Scale += glm::vec3(0.08f, 0.08f, 0.08f); //Of these two lines
	}
	if (key == ',') {
		Selector[CurrPiece]->Scale -= glm::vec3(0.08f, 0.08f, 0.08f);
		CurrPiece--;
		
		if (CurrPiece < 0) {
			CurrPiece = 9;
		}
		Selector[CurrPiece]->Scale += glm::vec3(0.08f, 0.08f, 0.08f);
	}

	if (key == 'x') {
		//std::cout << Selector[CurrPiece]->JointAngles[2] << std::endl;
		Selector[CurrPiece]->JointAngles[0] = Selector[CurrPiece]->JointAngles[0] + 1.0;
		if (Selector[CurrPiece]->JointAngles[0] > 360) {
			Selector[CurrPiece]->JointAngles[0] = 0;
		}
	}
	if (key == 'X') {
		std::cout << Selector[CurrPiece]->JointAngles[0] << std::endl;
		Selector[CurrPiece]->JointAngles[0] = Selector[CurrPiece]->JointAngles[0] - 1.0;
		if (Selector[CurrPiece]->JointAngles[0] < -360) {
			Selector[CurrPiece]->JointAngles[0] = 0;
		}
	}

	if (key == 'y') {
		std::cout << Selector[CurrPiece]->JointAngles[1] << std::endl;
		Selector[CurrPiece]->JointAngles[1] = Selector[CurrPiece]->JointAngles[1] + 1.0;
		if (Selector[CurrPiece]->JointAngles[1] > 360) {
			Selector[CurrPiece]->JointAngles[1] = 0;
		}
	}
	if (key == 'Y') {
		std::cout << Selector[CurrPiece]->JointAngles[1] << std::endl;
		Selector[CurrPiece]->JointAngles[1] = Selector[CurrPiece]->JointAngles[1] - 1.0;
		if (Selector[CurrPiece]->JointAngles[1] < -360) {
			Selector[CurrPiece]->JointAngles[1] = 0;
		}
	}

	if (key == 'z') {
		std::cout<<Selector[CurrPiece]->JointAngles[2] << std::endl;
		Selector[CurrPiece]->JointAngles[2] = Selector[CurrPiece]->JointAngles[2] +1.0;
		if (Selector[CurrPiece]->JointAngles[2] > 360) {
			Selector[CurrPiece]->JointAngles[2] = 0;
		}
	}
	if (key == 'Z') {
		std::cout << Selector[CurrPiece]->JointAngles[2] << std::endl;
		Selector[CurrPiece]->JointAngles[2] = Selector[CurrPiece]->JointAngles[2] - 1.0;
		if (Selector[CurrPiece]->JointAngles[2] < -360) {
			Selector[CurrPiece]->JointAngles[2] = 0;
		}
	}

	if (key == ' ') { //Reset Camara and center;
		eye = glm::vec3(0.0f, 0.0f, 8.0f);
		center =glm::vec3(0.0f, 0.0f, 0.0f);
		up = glm::vec3(0.0f, 1.0f, 0.0f);
	}
}

void CreateCube()
{
	// x, y, z, r, g, b, ...
	float cubeVerts[] = {
		// Face x-
		-1.0f,	+1.0f,	+1.0f,	0.8f,	0.2f,	0.2f,
		-1.0f,	+1.0f,	-1.0f,	0.8f,	0.2f,	0.2f,
		-1.0f,	-1.0f,	+1.0f,	0.8f,	0.2f,	0.2f,
		-1.0f,	-1.0f,	+1.0f,	0.8f,	0.2f,	0.2f,
		-1.0f,	+1.0f,	-1.0f,	0.8f,	0.2f,	0.2f,
		-1.0f,	-1.0f,	-1.0f,	0.8f,	0.2f,	0.2f,
		// Face x+
		+1.0f,	+1.0f,	+1.0f,	0.8f,	0.2f,	0.2f,
		+1.0f,	-1.0f,	+1.0f,	0.8f,	0.2f,	0.2f,
		+1.0f,	+1.0f,	-1.0f,	0.8f,	0.2f,	0.2f,
		+1.0f,	+1.0f,	-1.0f,	0.8f,	0.2f,	0.2f,
		+1.0f,	-1.0f,	+1.0f,	0.8f,	0.2f,	0.2f,
		+1.0f,	-1.0f,	-1.0f,	0.8f,	0.2f,	0.2f,
		// Face y-
		+1.0f,	-1.0f,	+1.0f,	0.2f,	0.8f,	0.2f,
		-1.0f,	-1.0f,	+1.0f,	0.2f,	0.8f,	0.2f,
		+1.0f,	-1.0f,	-1.0f,	0.2f,	0.8f,	0.2f,
		+1.0f,	-1.0f,	-1.0f,	0.2f,	0.8f,	0.2f,
		-1.0f,	-1.0f,	+1.0f,	0.2f,	0.8f,	0.2f,
		-1.0f,	-1.0f,	-1.0f,	0.2f,	0.8f,	0.2f,
		// Face y+
		+1.0f,	+1.0f,	+1.0f,	0.2f,	0.8f,	0.2f,
		+1.0f,	+1.0f,	-1.0f,	0.2f,	0.8f,	0.2f,
		-1.0f,	+1.0f,	+1.0f,	0.2f,	0.8f,	0.2f,
		-1.0f,	+1.0f,	+1.0f,	0.2f,	0.8f,	0.2f,
		+1.0f,	+1.0f,	-1.0f,	0.2f,	0.8f,	0.2f,
		-1.0f,	+1.0f,	-1.0f,	0.2f,	0.8f,	0.2f,
		// Face z-
		+1.0f,	+1.0f,	-1.0f,	0.2f,	0.2f,	0.8f,
		+1.0f,	-1.0f,	-1.0f,	0.2f,	0.2f,	0.8f,
		-1.0f,	+1.0f,	-1.0f,	0.2f,	0.2f,	0.8f,
		-1.0f,	+1.0f,	-1.0f,	0.2f,	0.2f,	0.8f,
		+1.0f,	-1.0f,	-1.0f,	0.2f,	0.2f,	0.8f,
		-1.0f,	-1.0f,	-1.0f,	0.2f,	0.2f,	0.8f,
		// Face z+
		+1.0f,	+1.0f,	+1.0f,	0.2f,	0.2f,	0.8f,
		-1.0f,	+1.0f,	+1.0f,	0.2f,	0.2f,	0.8f,
		+1.0f,	-1.0f,	+1.0f,	0.2f,	0.2f,	0.8f,
		+1.0f,	-1.0f,	+1.0f,	0.2f,	0.2f,	0.8f,
		-1.0f,	+1.0f,	+1.0f,	0.2f,	0.2f,	0.8f,
		-1.0f,	-1.0f,	+1.0f,	0.2f,	0.2f,	0.8f
	};

	GLuint vertBufferID;
	glGenBuffers(1, &vertBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW);
	GLint posID = glGetAttribLocation(program.GetPID(), "position");
	glEnableVertexAttribArray(posID);
	glVertexAttribPointer(posID, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	GLint colID = glGetAttribLocation(program.GetPID(), "color");
	glEnableVertexAttribArray(colID);
	glVertexAttribPointer(colID, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));

}

void FrameBufferSizeCallback(GLFWwindow* lWindow, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Init()
{
	glfwInit();
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Assignment2 - Aldo Leon Marquez", NULL, NULL);
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetMouseButtonCallback(window, MouseCallback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, CursorPositionCallback);
	glfwSetCharCallback(window, CharacterCallback);
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	program.SetShadersFileName("../shaders/shader.vert", "../shaders/shader.frag");
	program.Init();

	CreateCube();
}


int main()
{	
	ConstructRobot();
	Selector[CurrPiece]->Scale += glm::vec3(0.08f, 0.08f, 0.08f);
	Init();
	while ( glfwWindowShouldClose(window) == 0) 
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Display();
		glFlush();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}