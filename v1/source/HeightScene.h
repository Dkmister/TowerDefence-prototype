#ifndef __HEIGHT_SCENE_H__
#define __HEIGHT_SCENE_H__

#include "ThirdPersonCamera.h"
#include "FirstPersonCamera.h"
#include "ArcballCamera.h"

#include "Scene.h"
#include "Loaders.h"
#include "HeightMap.h"
#include "LightSource.h"

#include "glsl/ShaderProgram.h"

#include "glsl/VertexArrayObject.h"
#include "glsl/VertexBufferObject.h"

#include "gfx/ogldev_skinned_mesh.h"

class HeightScene : public Scene {
public:
   HeightScene();
   virtual ~HeightScene();

   void render(GLFWwindow* window);

protected:
   std::unique_ptr<Camera> _camera;
   
   std::unique_ptr<SkinnedMesh> _mesh;
   std::unique_ptr<HeightMap> _heightmap;
   std::unique_ptr<LightManager> _lights;

   std::unique_ptr<glsl::ShaderProgram> _shader;

   irrklang::ISound* _music;
   irrklang::ISoundEngine* _engine;

private:
};

#endif // __HEIGHT_SCENE_H__