#include "stdafx.h"
#include "ThirdPersonCamera.h"

ThirdtPersonCamera::ThirdtPersonCamera(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up)
   : Camera(eye, center, up)
{
   _pitch = 1.f;
   _glfwTime = -1.f;
}

ThirdtPersonCamera::~ThirdtPersonCamera() { }

void ThirdtPersonCamera::computeFromInputs(GLFWwindow* window) {
   if (_glfwTime == -1) _glfwTime = glfwGetTime();

   // Compute time difference between current and last frame
   double currentTime = glfwGetTime();
   float deltaTime = float(currentTime - _glfwTime);

   glm::vec3 target_to_camera = glm::vec3(_position - _target);

   glm::vec3 right = glm::normalize(glm::cross(target_to_camera, glm::vec3(0.f, 1.f, 0.f)));
   _up = glm::normalize(glm::cross(right, target_to_camera));

   float cosAngles = glm::dot(glm::vec3(0.f, 1.f, 0.f), glm::normalize(_position));
   if (cosAngles > 0.8 || cosAngles < 0.2) {
      _pitch *= -1;
   }

   float yaw_angle_radians = 30.f * deltaTime;
   float pitch_angle_radians = 5.f * deltaTime * _pitch;

   glm::mat4 rotate_yaw_matrix = glm::rotate(yaw_angle_radians, glm::vec3(0.f, 1.f, 0.f));
   glm::mat4 rotate_pitch_matrix = glm::rotate(pitch_angle_radians, right);
   
   //yaw, Y-up system
   target_to_camera = glm::vec3(rotate_yaw_matrix * (glm::vec4(target_to_camera, 0.f)));

   //pitch    
   target_to_camera = glm::vec3(rotate_pitch_matrix * (glm::vec4(target_to_camera, 0.f)));
   
   _position = target_to_camera + _target;   
   
   _glfwTime = currentTime;
}