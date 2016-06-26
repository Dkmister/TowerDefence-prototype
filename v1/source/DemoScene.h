#ifndef __DEMO_SCENE_H__
#define __DEMO_SCENE_H__

#include "FirstPersonCamera.h"
#include "ArcballCamera.h"

#include "Scene.h"
#include "Loaders.h"

#include "glsl/ShaderProgram.h"

#include "glsl/VertexArrayObject.h"
#include "glsl/VertexBufferObject.h"

class DemoScene : public Scene {
public:
   DemoScene();
   virtual ~DemoScene();

   void render(GLFWwindow* window);

protected:
   GLuint _size, _texture;
  
   std::unique_ptr<Camera> camera;

   std::unique_ptr<glsl::ShaderProgram> _shader;

   std::unique_ptr<glsl::VertexArrayObject> _vao;
   std::unique_ptr<glsl::VertexBufferObject> _vbo_data, _vbo_uv;   
};

#endif // __DEMO_SCENE_H__