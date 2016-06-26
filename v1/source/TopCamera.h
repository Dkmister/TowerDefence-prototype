#ifndef __TOP_CAMERA_H__
#define __TOP_CAMERA_H__

#include "Camera.h"

class TopCamera : public Camera {
public:
	TopCamera(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up);
	virtual ~TopCamera();

	virtual void computeFromInputs(GLFWwindow* window);

private:
	float _keyboardSpeed;
	float _mouseSpeed;

	double _lastXPos, _lastYPos;
	double _glfwTime;

	float _horizontalAngle, _verticalAngle;
};

#endif // __TOP_CAMERA_H__