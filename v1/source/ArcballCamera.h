#ifndef __ARCBALL_CAMERA_H__
#define __ARCBALL_CAMERA_H__

#include "Camera.h"

class ArcballCamera : public Camera {
public:
   ArcballCamera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up);
   virtual ~ArcballCamera();

   virtual void computeFromInputs(GLFWwindow* window);

private:
   glm::vec3 get_arcball_vector(GLFWwindow* window, double x, double y);

   float _keyboardSpeed;
   float _mouseSpeed;

   double _lastXPos, _lastYPos;
   double _glfwTime;
};

#endif // __ARCBALL_CAMERA_H__