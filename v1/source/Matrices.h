#ifndef __MATRICES_H__
#define __MATRICES_H__

#include "glsl/ShaderProgram.h"

 namespace util {
	 
	//renderização por matriz em pilha
   static void updateMVP(glsl::ShaderProgram& program, const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection) {
      glm::mat4 ModelView(view * model);
      glm::mat4 Normal(glm::inverseTranspose(glm::mat3(ModelView)));      
      glm::mat4 ModelViewProjection(projection * view * model);

      glUniformMatrix4fv(program.getUniformLocation("NormalMatrix"), 1, GL_FALSE, &Normal[0][0]);
      glUniformMatrix4fv(program.getUniformLocation("ModelMatrix"), 1, GL_FALSE, &model[0][0]);
      glUniformMatrix4fv(program.getUniformLocation("ModelViewMatrix"), 1, GL_FALSE, &ModelView[0][0]);
      glUniformMatrix4fv(program.getUniformLocation("ModelViewProjectionMatrix"), 1, GL_FALSE, &ModelViewProjection[0][0]);
   }
} // namespace util

#endif // __MATRICES_H__