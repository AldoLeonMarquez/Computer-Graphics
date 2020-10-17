#version 140

attribute vec3 vPositionModel; // in object space position of the 
attribute vec3 vNormalModel; // in object space
uniform vec3 eye;

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

void main()
{
	
	vec4 Position = projection * view * model * vec4(vPositionModel, 1.0);  //No projection, no view 
	vec4 P =  model * vec4(vPositionModel, 1.0);  //No projection, no view 
	vec4 N = inverse(transpose(model)) * normalize(vec4(vNormalModel,0.0f));
	
	vec3 E = normalize(eye-vec3(P[0],P[1],P[2]) );
	for(int i=0; i < 2 ;i++){
		vec3 L = normalize(lights[i].position - vec3(P[0],P[1],P[2]));
		float LN = dot( L , vec3(N[0],N[1],N[2]));
		vec3 R = ((2.0f*LN) * vec3(N[0],N[1],N[2])) - L;
		vec3 S = ks*(pow(max(0,dot(R,E)),s));
		color += lights[i].color * (kd * max(0,LN) + S );
	}

	gl_Position = Position;  //fix

	color += ka;
}