#ifndef __SCENE_H__
#define __SCENE_H__

class Scene {
public:
   Scene() {};
   virtual ~Scene() {};
   virtual void render(GLFWwindow* window) = 0;

protected:
	
private:
};

#endif // __SCENE_H__