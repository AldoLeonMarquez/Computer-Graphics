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
uniform float CosTheta;

struct lightStruct
{
	vec3 position;
	vec3 color;
};
uniform lightStruct lights[2];

void main()
{
	
	vec3 I = vec3(0.0f,0.0f,0.0f);

	vec4 PM = model * VPM; //position of model in World Coordinates
	vec4 NM =inverse(transpose(model)) *  normalize(VNM);  //position of model in World Coordinates
	vec3 Eye = normalize(eye-vec3(PM[0],PM[1],PM[2]));

	for(int i=0; i <2;i++){
		vec3 L = normalize(lights[i].position - vec3(PM[0],PM[1],PM[2])); //Light 
		float LN = dot( L , vec3(NM[0],NM[1],NM[2])); //L and N dot Product 
		vec3 R = ((2.0f*LN) * vec3(NM[0],NM[1],NM[2])) - L; //Reflected 
		vec3 S = ks*(pow(max(0,dot(R,Eye)),s)); //Specular 

		float spot = dot(L,normalize(lights[i].position));
		if( spot >= CosTheta){
			spot = pow(spot,70.0f); //medium alpha
			
		}

		I += lights[i].color * (kd * max(0,LN) + S)*spot;
	}
	
	

	I+=ka;
	gl_FragColor = vec4(I, 1.0f);
}
