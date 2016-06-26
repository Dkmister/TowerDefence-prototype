#ifndef __FIRST_PERSON_CAMERA_H__
#define __FIRST_PERSON_CAMERA_H__

#include "Camera.h"

class FirstPersonCamera : public Camera {
public:
	FirstPersonCamera(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up);
	virtual ~FirstPersonCamera();

   virtual void computeFromInputs(GLFWwindow* window);

private:
   float _keyboardSpeed;
   float _mouseSpeed;

   double _lastXPos, _lastYPos;
   double _glfwTime;

   float _horizontalAngle, _verticalAngle;
};

#endif // __FIRST_PERSON_CAMERA_H__