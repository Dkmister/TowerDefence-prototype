#version 330 core

uniform mat4 NormalMatrix;
uniform mat4 ModelMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 ModelViewProjectionMatrix;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;
layout (location = 2) in vec3 inNormal;

smooth out vec2 vTexCoord;
smooth out vec3 vNormal;
smooth out vec3 vWorldPos;

void main() {
   vTexCoord = inCoord;
	vNormal = inNormal;
   
   vWorldPos = (ModelMatrix * vec4(inPosition, 1.0)).xyz;
   gl_Position = ModelViewProjectionMatrix * vec4(inPosition, 1.0);
}