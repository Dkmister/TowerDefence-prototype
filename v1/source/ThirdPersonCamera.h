#ifndef __THIRD_PERSON_CAMERA_H__
#define __THIRD_PERSON_CAMERA_H__

#include "Camera.h"

class ThirdtPersonCamera : public Camera {
public:
   ThirdtPersonCamera(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up);
   virtual ~ThirdtPersonCamera();

   virtual void computeFromInputs(GLFWwindow* window);

private:
   double _glfwTime;
   float _pitch;
};

#endif // __THIRD_PERSON_CAMERA_H__