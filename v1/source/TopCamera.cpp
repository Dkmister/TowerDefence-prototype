#include "stdafx.h"
#include "TopCamera.h"

#define PI 3.14159265359
#define XMIN 500
#define XMAX -500
#define YMIN 500
#define YMAX -500
//#define WHAT 2

TopCamera::TopCamera(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up)
	: Camera(eye, center, up)
{
	_keyboardSpeed = 400.0f;
	_mouseSpeed = 0.002f;

	_lastXPos = 0.f;
	_lastYPos = 0.f;
	_glfwTime = -1.f;

	// Initial horizontal angle : toward -Z
	_horizontalAngle = 1*PI; // PI
												// Initial vertical angle : none
	_verticalAngle = -0.5*PI;
}

TopCamera::~TopCamera() { }

void TopCamera::computeFromInputs(GLFWwindow* window) {
	if (_glfwTime == -1) _glfwTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - _glfwTime);

	// Get mouse position

	double currentXpos = _lastXPos, currentYpos = _lastYPos;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
		glfwGetCursorPos(window, &currentXpos, &currentYpos);
	}

	// Compute new orientation
	_horizontalAngle += _mouseSpeed * float(_lastXPos - currentXpos);
	_verticalAngle += _mouseSpeed * float(_lastYPos - currentYpos);

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(_verticalAngle) * sin(_horizontalAngle),
		sin(_verticalAngle),
		cos(_verticalAngle) * cos(_horizontalAngle)
		);

	// Right vector
	glm::vec3 right = glm::vec3(sin(_horizontalAngle - 3.14f / 2.0f), 0, cos(_horizontalAngle - 3.14f / 2.0f));

	// Up vector
	_up = glm::cross(right, direction);


	// Strafe right
	if (glfwGetKey(window, 'D') == GLFW_PRESS && _position[0]<XMIN) {
		_position += right * deltaTime * _keyboardSpeed;
		//printf("%2.2f\n", _position[WHAT]);
	}
	// Strafe left
	if (glfwGetKey(window, 'A') == GLFW_PRESS && _position[0]>XMAX) {
		_position -= right * deltaTime * _keyboardSpeed;
		//printf("%2.2f\n",_position[WHAT]);
	}

	// Move Up
	if (glfwGetKey(window, 'W') == GLFW_PRESS && _position[2]>YMAX) {
		_position += _up * deltaTime * _keyboardSpeed;
		//printf("%2.2f\n", _position[WHAT]);
	}
	// Move Down
	if (glfwGetKey(window, 'S') == GLFW_PRESS && _position[2]<YMIN) {
		_position -= _up * deltaTime * _keyboardSpeed;
		//printf("%2.2f\n", _position[WHAT]);
	}

	_target = _position + direction;

	_glfwTime = currentTime;
	glfwGetCursorPos(window, &_lastXPos, &_lastYPos);
}
