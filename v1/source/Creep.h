#pragma once
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




class Creep
{
public:
	Creep();
	~Creep();
	double posxy[2];
	void render();
	void changeHp(int damage);
	void move();

private:
	int hp = 50;
	double speed = 0.05;
	

};

