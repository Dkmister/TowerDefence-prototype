#ifndef __LIGHT_SOURCE_H__
#define __LIGHT_SOURCE_H__

#include "glsl\UniformBufferObject.h"
#include "glsl\ShaderProgram.h"

class LightManager {
public:
   struct Light {
      glm::vec4 position;
      glm::vec4 ambient;
      glm::vec4 diffuse;
      glm::vec4 specular;

      glm::vec4 spot_direction;
      GLfloat spotCutoff, spotExponent;

      GLfloat constantAttenuation, linearAttenuation, quadraticAttenuation;
      float padding[3];
   };  

   LightManager(GLuint numberOfLights);
   ~LightManager(void) {}

   void computePositionFromInputs(GLFWwindow* window, int key);
   void updateData(glsl::ShaderProgram& program, const glm::mat4 &lViewMatrix);

   inline Light * getLight(GLuint index);

   inline GLint getNumberOfLights(void) const;

private:
   GLuint _selectedLight;
   GLuint _numberOfLights;
   std::vector<Light> _data;

   // Initial horizontal angle : toward -Z
   static float horizontalAngle;
   // Initial vertical angle : none
   static float verticalAngle;

   static float speed;
   static float mouseSpeed;

   static double lastXpos, lastYpos;
};


LightManager::Light * LightManager::getLight(GLuint index) {
   if (index >= _numberOfLights) index = _numberOfLights - 1;
   return &_data[index];
}

GLint LightManager::getNumberOfLights(void) const {
   return _numberOfLights;
}

#endif // __LIGHT_SOURCE_H__