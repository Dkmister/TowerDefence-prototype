#version 330 core

// based on: http://www.opengl.org/sdk/docs/tutorials/ClockworkCoders/lighting.php
// and http://www.opengl-tutorial.org/beginners-tutorials/tutorial-8-basic-shading/

#define Max_Lights 10

struct MaterialParameters {
   vec4 ambient;     // Acm
   vec4 diffuse;     // Dcm
   vec4 specular;    // Scm
   vec4 emission;    // Ecm
   float shininess;  // Srm   
   float opacity;
   int texCount;
};

struct LightSourceParameters {   
	vec4 ambient;        // Aclarri
	vec4 diffuse;        // Dcli
	vec4 specular;       // Scli
   vec4 position;       // Ppli
	vec4 spotDirection;  // Sdli
	float spotExponent;  // Srli
	float spotCutoff;    // Crli                              
                        // (range: [0.0,90.0], 180.0	
	float constantAttenuation;    // K0
	float linearAttenuation;      // K1
	float quadraticAttenuation;   // K2
};

// Values that stay constant for the whole mesh.
uniform sampler2D texUnit;
uniform int number_of_lights;
uniform MaterialParameters material;
uniform LightSourceParameters light[Max_Lights];

// Interpolated values from the vertex shaders
in vec3 position;
in vec3 normal;
in vec2 texcoord;

// Ouput data
out vec4 fragColor;

void main(void) {
   // Distance to the light
   float d = length(light[0].position.xyz - position);

   float attenuation = 1.0 / (light[0].constantAttenuation
				+ light[0].linearAttenuation * d
				+ light[0].quadraticAttenuation * d * d);

   // Normal of the computed fragment, in camera space
	vec3 n = normalize(normal);
    
   // Direction of the light (from the fragment to the light)
   vec3 l = normalize(light[0].position.xyz - position);

   // Cosine of the angle between the normal and the light direction, 
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta = clamp(dot(n, l), 0.0, 1.0);

   // Eye vector (towards the camera)
	vec3 e = normalize(-position); // we are in Eye Coordinates, so EyePos is (0,0,0)

   // Direction in which the triangle reflects the light
	vec3 r = normalize(reflect(-l, n));

   // Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha = clamp(dot(e, r), 0.0, 1.0);

   // calculate Ambient Term
   // Ambient : simulates indirect lighting
   vec4 Iamb = clamp(light[0].ambient * material.ambient, 0.0, 1.0);

	// calculate Diffuse Term
   // Diffuse : "color" of the object
	vec4 Idiff = light[0].diffuse * cosTheta;
	if (material.texCount > 0) {
		Idiff *= texture(texUnit, texcoord);		
	} else {		
		Idiff *= material.diffuse;
	}
	Idiff = clamp(Idiff * attenuation, 0.0, 1.0);

	// calculate Specular Term
   // Specular : reflective highlight, like a mirror
	vec4 Ispec = light[0].specular * material.specular * pow(cosAlpha, material.shininess);
	Ispec = clamp(Ispec * attenuation, 0.0, 1.0);
	
	// write Total Color
	fragColor = clamp(Iamb + Idiff + Ispec, 0.0, 1.0);
	fragColor.w = material.opacity;
}