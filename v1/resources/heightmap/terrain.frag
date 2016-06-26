#version 330 core

smooth in vec2 vTexCoord;
smooth in vec3 vNormal;
smooth in vec3 vWorldPos;

uniform sampler2D gSampler[6];

uniform float fMaxTextureU;
uniform float fMaxTextureV;

uniform vec3 fScale;

out vec4 outputColor;

void main() {
	vec3 vNormalized = normalize(vNormal);
	
	vec4 vTexColor = vec4(0.0);
	
   // scale
	float fScale = vWorldPos.y / fScale.y;

   const float fRange0 = 0.0f;
	const float fRange1 = 0.15f;
	const float fRange2 = 0.3f;
	const float fRange3 = 0.65f;
   const float fRange4 = 0.85f;

	if (fScale >= 0.0 && fScale <= fRange1) {
      fScale -= fRange0;
		fScale /= (fRange1-fRange0);
		
		float fScale2 = fScale;
		fScale = 1.0-fScale; 
		
		vTexColor += texture2D(gSampler[0], vTexCoord)*fScale;
		vTexColor += texture2D(gSampler[3], vTexCoord)*fScale2;

   } else if (fScale <= fRange2) {
		fScale -= fRange1;
		fScale /= (fRange2-fRange1);
		
		float fScale2 = fScale;
		fScale = 1.0-fScale; 
		
		vTexColor += texture2D(gSampler[3], vTexCoord)*fScale;
		vTexColor += texture2D(gSampler[1], vTexCoord)*fScale2;

	} else if (fScale <= fRange3) {
      vTexColor = texture2D(gSampler[1], vTexCoord);

   } else if(fScale <= fRange4) {
		fScale -= fRange3;
		fScale /= (fRange4-fRange3);
		
		float fScale2 = fScale;
		fScale = 1.0-fScale; 
		
		vTexColor += texture2D(gSampler[1], vTexCoord)*fScale;
		vTexColor += texture2D(gSampler[2], vTexCoord)*fScale2;		

	} else {
      vTexColor = texture2D(gSampler[2], vTexCoord);
   }

	vec2 vPathCoord = vec2(vTexCoord.x/fMaxTextureU, vTexCoord.y/fMaxTextureV);

   // Black color means there is a path
	vec4 vPathIntensity = texture2D(gSampler[4], vPathCoord);
	fScale = vPathIntensity.x;
  
	vec4 vPathColor = texture2D(gSampler[5], vTexCoord); 
   
	outputColor = fScale * vTexColor + (1 - fScale) * vPathColor;
}                      