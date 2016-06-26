#ifndef __CAMERA_H__
#define __CAMERA_H__

class Camera {
public:
   Camera(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up)
      : _position(eye), _target(center), _up(up) { };
   
	virtual ~Camera() { };

   virtual void computeFromInputs(GLFWwindow* window) = 0;

   inline void set(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up) {
      _position = eye;
      _target = center;
      _up = up;
   };

   inline const glm::vec3& getEye() const {
      return _position;
   };
   inline const glm::vec3& getCenter() const {
      return _target;
   };
   inline const glm::vec3& getUp() const {
      return _up;
   };

   inline glm::mat4 getMatrix() const {
      return glm::lookAt(_position, _target, _up);
   };

protected:
   glm::vec3 _position;
   glm::vec3 _target;
   glm::vec3 _up;	
};

#endif // __CAMERA_H__