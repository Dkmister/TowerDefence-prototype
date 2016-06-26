#include "stdafx.h"
#include "ArcballCamera.h"

ArcballCamera::ArcballCamera(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up)
   : Camera(eye, center, up)
{
   _keyboardSpeed = 25.0f;
   _mouseSpeed = 20.f;

   _lastXPos = 0.f;
   _lastYPos = 0.f;
   _glfwTime = -1.f;
}

ArcballCamera::~ArcballCamera()
{

}

void ArcballCamera::computeFromInputs(GLFWwindow* window)
{
   if (_glfwTime == -1) _glfwTime = glfwGetTime();

   // Compute time difference between current and last frame
   double currentTime = glfwGetTime();
   float deltaTime = float(currentTime - _glfwTime);

   double currentXpos, currentYpos;

   if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
      glfwGetCursorPos(window, &currentXpos, &currentYpos);

      glm::vec3 va = get_arcball_vector(window, _lastXPos, _lastYPos);
      glm::vec3 vb = get_arcball_vector(window, currentXpos,  currentYpos);

      float angle = glm::acos(glm::min(1.0f, glm::dot(va, vb)));

      glm::vec3 axis_in_camera_coord = glm::cross(va, vb);

      if (glm::length(axis_in_camera_coord) > 1E-6) {

         glm::mat4 matrix = glm::rotate(glm::degrees(angle), axis_in_camera_coord);
         _position = glm::vec3(matrix * glm::vec4(_position, 1.0));

         glm::vec3 target_to_camera = glm::normalize(_position - _target); 
         glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), target_to_camera));
         _up = glm::cross(target_to_camera, right);
      }      
   }

   _glfwTime = currentTime;
   glfwGetCursorPos(window, &_lastXPos, &_lastYPos);
}

glm::vec3 ArcballCamera::get_arcball_vector(GLFWwindow* window, double x, double y)
{
   int width, height;
   glfwGetWindowSize(window, &width, &height);

   glm::vec3 coord((2 * x - width ) / width, -(2 * y - height) / height, 0);
   
   float length_squared  = coord.x * coord.x + coord.y * coord.y;
   if (length_squared  <= 1.f) {
      coord.z = glm::sqrt(1.f - length_squared);
   } else {
      coord = glm::normalize(coord);
   }
   return coord;
}
