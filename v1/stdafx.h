// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS

// GLEW
#include <GL/gl3w.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#define GLM_FORCE_INLINE

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

// Assimp
#define AI_CONFIG_PP_PTV_NORMALIZE   "PP_PTV_NORMALIZE"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

// FreeImage
#include <FreeImage.h>

// irrKlang
#include <irrKlang/irrKlang.h>

#include <cstdlib>
#include <cstdio>

#include <algorithm>

#include <exception>
#include <stdexcept>

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <memory>

#include <string>
#include <vector>
#include <map>

#include "gfx/string.h"

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&);   \
    void operator=(const TypeName&)

#pragma warning( disable : 4290)

typedef unsigned int uint;
#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }
#define GLCheckError() (glGetError() == GL_NO_ERROR)

// TODO: reference additional headers your program requires here
