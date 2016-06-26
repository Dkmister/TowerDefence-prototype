#version 330 core

#define Max_Bones 100

uniform mat4 NormalMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 ModelViewProjectionMatrix;

uniform int has_animations;

uniform mat4 gBones[Max_Bones];

layout (location = 0) in vec3 in_vertex;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texcoord;
layout (location = 3) in ivec4 BoneIDs;
layout (location = 4) in vec4 Weights;

out vec3 position;
out vec3 normal;
out vec2 texcoord;

void main(void) {
   mat4 BoneTransform = mat4(1);
   if (has_animations == 1) {
      BoneTransform = gBones[BoneIDs[0]] * Weights[0];
      BoneTransform += gBones[BoneIDs[1]] * Weights[1];
      BoneTransform += gBones[BoneIDs[2]] * Weights[2];
      BoneTransform += gBones[BoneIDs[3]] * Weights[3];
   }

   vec4 PosL    = BoneTransform * vec4(in_vertex, 1.0);
   gl_Position  = ModelViewProjectionMatrix * PosL;

   texcoord = (in_texcoord).xy;

   vec4 NormalL = BoneTransform * vec4(in_normal, 0.0);
   normal = normalize(NormalMatrix * NormalL).xyz;
      
   position = (ModelViewMatrix * PosL).xyz;
}