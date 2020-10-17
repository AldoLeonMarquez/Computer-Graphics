#version 140

attribute vec3 vPositionModel; // in object space position of the 
attribute vec3 vNormalModel; // in object space

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

struct lightStruct
{
	vec3 position;
	vec3 color;
};

#define NUM_LIGHTS 2

uniform lightStruct lights[NUM_LIGHTS];

uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float s;

varying vec3 color;

varying vec4 VNM;
varying vec4 VPM;

uniform float CosTheta;

void main()
{
	
	VPM = vec4(vPositionModel,1.0f);  
	VNM = vec4(vNormalModel,0.0f);
	vec4 Position = projection * view * model * vec4(vPositionModel, 1.0);  //No projection, no view 

	gl_Position = Position;  

}