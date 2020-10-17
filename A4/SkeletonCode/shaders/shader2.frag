#version 140

varying vec4 VPM; // in object space position of the 
varying vec4 VNM; // in object space
uniform vec3 eye;

varying vec3 color; 

uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float s;

uniform mat4 model;


struct lightStruct
{
	vec3 position;
	vec3 color;
};
uniform lightStruct lights[2];

void main()
{
	
	vec3 I ;

	vec4 PM = model * VPM; //position of model in World Coordinates
	vec4 NM =inverse(transpose(model)) *  normalize(VNM);  //position of model in World Coordinates
	vec3 Eye = normalize(eye-vec3(PM[0],PM[1],PM[2]));

	float Angle = degrees(acos(dot(vec3(NM[0],NM[1],NM[2]),Eye)));
	if(Angle >= 75.0f && Angle <= 90.0f){
		I = vec3(1.0f,1.0f,1.0f) * radians(Angle);
	}
	else{
		I = vec3(0.0f,0.0f,0.0f);
	}
	gl_FragColor = vec4(I, 1.0f);
}
