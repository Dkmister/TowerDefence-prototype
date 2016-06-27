#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "ThirdPersonCamera.h"
#include "FirstPersonCamera.h"
#include "ArcballCamera.h"
#include "TopCamera.h"

#include "Scene.h"
#include "Loaders.h"
#include "HeightMap.h"
#include "LightSource.h"

#include "glsl/ShaderProgram.h"

#include "glsl/VertexArrayObject.h"
#include "glsl/VertexBufferObject.h"

#include "gfx/ogldev_skinned_mesh.h"

class GameScene : public Scene {
public:
	GameScene();
	virtual ~GameScene();

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
	float tempoJogo = 0; //em segundos
	double bob_pos[2] = {0,0};

};

#endif // __GAME_SCENE_H__
