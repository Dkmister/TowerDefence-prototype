#version 330 core

uniform mat4 NormalMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 ModelViewProjectionMatrix;

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 in_vertex;
layout(location = 1) in vec2 in_texcoord;

// Output data ; will be interpolated for each fragment.
out vec2 UV;

void main(){

	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  ModelViewProjectionMatrix * vec4(in_vertex, 1);
	
	// UV of the vertex. No special space for this one.
	UV = in_texcoord;
}

