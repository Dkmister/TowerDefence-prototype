#include "stdafx.h"
#include "LightSource.h"

float LightManager::horizontalAngle = 3.14f;
float LightManager::verticalAngle = 0.0f;

float LightManager::speed = 10.0f;
float LightManager::mouseSpeed = 0.002f;

double LightManager::lastXpos = 0.0;
double LightManager::lastYpos = 0.0;

LightManager::LightManager(GLuint numberOfLights)  : _selectedLight(0) {
   if (numberOfLights < 1) _numberOfLights = 1;
   else _numberOfLights = std::min((int)numberOfLights, 10);

   _data.resize(_numberOfLights);   
}

void LightManager::computePositionFromInputs(GLFWwindow* window, int key) {
   if (key >= 49 && key <= 57){
      _selectedLight = key - GLFW_KEY_1;
   }
   
   // glfwGetTime is called only once, the first time this function is called
   static double lastTime = glfwGetTime();

   // Compute time difference between current and last frame
   double currentTime = glfwGetTime();
   float deltaTime = float(currentTime - lastTime);

   // Get mouse position

   double currentXpos = lastXpos, currentYpos = lastYpos;

   // Compute new orientation
   horizontalAngle += mouseSpeed * float(lastXpos - currentXpos);
   verticalAngle   += mouseSpeed * float(lastYpos - currentYpos);

   // Direction : Spherical coordinates to Cartesian coordinates conversion
   glm::vec3 direction(
      cos(verticalAngle) * sin(horizontalAngle),
      sin(verticalAngle),
      cos(verticalAngle) * cos(horizontalAngle)
      );

   // Right vector
   glm::vec3 right = glm::vec3(
      sin(horizontalAngle - 3.14f/2.0f),
      0,
      cos(horizontalAngle - 3.14f/2.0f)
      );

   // Up vector
   glm::vec3 up = glm::cross(right, direction);

   // Move forward
   if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS){
      _data[_selectedLight].position += glm::vec4(direction * deltaTime * speed, 0.0);
   }
   // Move backward
   if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS){
      _data[_selectedLight].position -= glm::vec4(direction * deltaTime * speed, 0.0);
   }
   // Strafe right
   if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS){
      _data[_selectedLight].position += glm::vec4(right * deltaTime * speed, 0.0);
   }
   // Strafe left
   if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS){
      _data[_selectedLight].position -= glm::vec4(right * deltaTime * speed, 0.0);
   }

   // Move Up
   if (glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_PRESS){
      _data[_selectedLight].position += glm::vec4(up * deltaTime * speed, 0.0);
   }
   // Move Down
   if (glfwGetKey(window, GLFW_KEY_KP_9) == GLFW_PRESS){
      _data[_selectedLight].position -= glm::vec4(up * deltaTime * speed, 0.0);
   }

   lastTime = currentTime;
   glfwGetCursorPos(window, &lastXpos, &lastYpos);
}

void LightManager::updateData(glsl::ShaderProgram& program, const glm::mat4 &ViewMatrix) {
    
    glUniform1i(program.getUniformLocation("number_of_lights"), _numberOfLights);

    for (GLuint l = 0; l < _numberOfLights; ++l) {
        //getLight(l)->position = ViewMatrix * glm::vec4(getData(l)->position, 1.0);
        //getLight(l)->spotDirection = glm::normalize(ViewMatrix * glm::vec4(getData(l)->spot_direction, 0.0));
      
       glm::vec4 position(ViewMatrix * getLight(l)->position);
       glm::vec4 spot_direction(glm::normalize(ViewMatrix * getLight(l)->spot_direction));

		glUniform4fv(program.getUniformLocation("light[" + util::string(l) + "].position"), 1, &position[0]);
      glUniform4fv(program.getUniformLocation("light[" + util::string(l) + "].spotDirection"), 1, &spot_direction[0]);

		glUniform4fv(program.getUniformLocation("light[" + util::string(l) + "].ambient"), 1, &getLight(l)->ambient[0]);
		glUniform4fv(program.getUniformLocation("light[" + util::string(l) + "].diffuse"), 1, &getLight(l)->diffuse[0]);
		glUniform4fv(program.getUniformLocation("light[" + util::string(l) + "].specular"), 1, &getLight(l)->specular[0]);
		
		glUniform1f(program.getUniformLocation("light[" + util::string(l) + "].spotCutoff"), getLight(l)->spotCutoff);
		glUniform1f(program.getUniformLocation("light[" + util::string(l) + "].spotExponent"), getLight(l)->spotExponent);

		glUniform1f(program.getUniformLocation("light[" + util::string(l) + "].constantAttenuation"), getLight(l)->constantAttenuation);
		glUniform1f(program.getUniformLocation("light[" + util::string(l) + "].linearAttenuation"), getLight(l)->linearAttenuation);
		glUniform1f(program.getUniformLocation("light[" + util::string(l) + "].quadraticAttenuation"), getLight(l)->quadraticAttenuation);
    }

}
