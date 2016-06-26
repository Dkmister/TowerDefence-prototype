#version 330 core

layout(points) in;
layout(triangle_strip) out;
layout(max_vertices = 12) out;

uniform mat4 NormalMatrix;
uniform mat4 ModelMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 ModelViewProjectionMatrix;

uniform float fTimePassed;
uniform vec3 fScale;

smooth out vec2 vTexCoord;


uint hash( uint x ) {
    x += ( x << 10u );
    x ^= ( x >>  6u );
    x += ( x <<  3u );
    x ^= ( x >> 11u );
    x += ( x << 15u );
    return x;
}

float randomFloat(float f) {
    const uint mantissaMask = 0x007FFFFFu;
    const uint one          = 0x3F800000u;
    
    uint h = hash( floatBitsToUint( f ) );
    h &= mantissaMask;
    h |= one;
    
    float  r2 = uintBitsToFloat( h );
    return r2 - 1.0;
}

float randomFloat(float seed, float min, float max) {
	return min + randomFloat(seed) * (max- min);
}

int randomInt(float seed, int min, int max) {
	return int(float(min) + randomFloat(seed) * float(max-min));
}

void main() {
   const float fWindStrength = 4.0;
   const vec3 vWindDirection = normalize(vec3(1.0, 0.0, 1.0));

   const float minGrassPatchSize = 5.0;
   const float maxGrassPatchSize = 10.0;
   
	const float fRange0 = 0.0f;
	const float fRange1 = 0.15f;
	const float fRange2 = 0.3f;
	const float fRange3 = 0.65f;
	const float fRange4 = 0.85f;

	vec3 vGrassFieldPos = gl_in[0].gl_Position.xyz;

   float fScale = ((ModelMatrix * vec4(vGrassFieldPos, 1.0)).y / fScale.y);

   float fGrassPatchSize;

   if (fScale < fRange2 || fScale > fRange4) {
      return;
   } else if (fScale <= fRange3) {
   	fScale -= fRange2;
		fScale /= (fRange3-fRange2);
		
		float fScale2 = fScale;
		fScale = 1.0-fScale; 

      fGrassPatchSize = (minGrassPatchSize * fScale) + (maxGrassPatchSize * fScale2);
   } else {
      fScale -= fRange3;
		fScale /= (fRange4-fRange3);
		
		float fScale2 = fScale;
		fScale = 1.0-fScale; 

      fGrassPatchSize = (maxGrassPatchSize * fScale) + (minGrassPatchSize * fScale2);
   }

	vec3 vBaseDir[3];
	vBaseDir[0] = vec3(1.0, 0.0, 0.0);
	vBaseDir[1] = vec3(float(cos(radians(45.f))), 0.0f, float(sin(radians(45.f))));
	vBaseDir[2] = vec3(float(cos(radians(-45.f))), 0.0f, float(sin(radians(-45.f))));
	
   float fWindPower = sin((vGrassFieldPos.x / 30.f) + (vGrassFieldPos.z / 30.f) + (fTimePassed * (fWindStrength / 5.f)));		
	fWindPower *= fWindStrength;

   vec3 wind = (vWindDirection * fWindPower);   

	for(int i = 0; i < 3; i++) {		
      int iGrassPatch = randomInt((vGrassFieldPos.x * vGrassFieldPos.z) + i, 0, 3);
      	
		float fTCStartX = float(iGrassPatch) * 0.25f;
		float fTCEndX = fTCStartX + 0.25f;

      vec3 base = (vBaseDir[i] * fGrassPatchSize * 0.5f);

		// Grass patch bottom left vertex
		vec3 vBL = vGrassFieldPos - base;
		gl_Position = ModelViewProjectionMatrix * vec4(vBL, 1.0);
		vTexCoord = vec2(fTCStartX, 1.0);
		EmitVertex();

		// Grass patch top left vertex
		vec3 vTL = vGrassFieldPos - base + wind;
		vTL.y += fGrassPatchSize;   
		gl_Position = ModelViewProjectionMatrix * vec4(vTL, 1.0);
		vTexCoord = vec2(fTCStartX, 0.0);
		EmitVertex();

		// Grass patch bottom right vertex
		vec3 vBR = vGrassFieldPos + base;
		gl_Position = ModelViewProjectionMatrix * vec4(vBR, 1.0);
		vTexCoord = vec2(fTCEndX, 1.0);
		EmitVertex();

		// Grass patch top right vertex
		vec3 vTR = vGrassFieldPos + base + wind;
		vTR.y += fGrassPatchSize;  
		gl_Position = ModelViewProjectionMatrix * vec4(vTR, 1.0);
		vTexCoord = vec2(fTCEndX, 0.0);
		EmitVertex();
		
		EndPrimitive();
	}
}