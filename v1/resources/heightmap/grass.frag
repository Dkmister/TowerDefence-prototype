#version 330 core

smooth in vec2 vTexCoord;

out vec4 outputColor;

uniform sampler2D gSampler;

void main() {
   const float fAlphaTest = 0.25;

	vec4 vTexColor = texture(gSampler, vTexCoord);

	if(vTexColor.a < fAlphaTest)
		discard;

   vTexColor.rgb *= 1.25;
	
	outputColor = vTexColor;
}