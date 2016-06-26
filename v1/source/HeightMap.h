#ifndef __HEIGHT_MAP_H__
#define __HEIGHT_MAP_H__

#include "glsl/TextureObject.h"

#include "glsl/VertexArrayObject.h"
#include "glsl/VertexBufferObject.h"

#include "glsl/Shader.h"
#include "glsl/ShaderProgram.h"

#include "Camera.h"

class HeightMap {
public:
	HeightMap(const std::string path);
	virtual ~HeightMap();

   bool load();
   void release();

   void render(const Camera& camera, const glm::mat4& projection, bool grass);

   inline int getWidth() const;
   inline int getHeight() const;

   inline glm::vec3 getHeightFromVertexData(int x, int y);

protected:
   bool _loaded;

   std::string _path;
   int _height, _width;

   int _numGrassTrinagles;

   glm::vec3 _scale;

   std::vector<glm::vec3> vertex_data;
   std::vector<glm::vec3> vbo_grass_data;

   std::vector<glsl::VAO*> _vaos;
   std::vector<glsl::VBO*> _vbos;

   std::vector<glsl::ShaderProgram*> _programs;
   std::vector<glsl::Texture2D*> _textures;

   struct SortDepth {
      bool sort(const glm::vec3& eye) {
         if (this->eye != eye) {
            this->eye = eye;
            return true;
         }
         return false;
      }
      bool operator () (const glm::vec3& a, const glm::vec3& b) {
         return glm::distance(eye, a) > glm::distance(eye, b);
      }

      glm::vec3 eye;
   } sort_depth;
   
};

//////////////////////////////////////////////////////////////////////////////////////

int HeightMap::getWidth() const {
   return _width;
}

int HeightMap::getHeight() const {
   return _height;
}

glm::vec3 HeightMap::getHeightFromVertexData(int x, int y) {
   return vertex_data[std::max(0, std::min(getWidth() - 1, x)) * _width + std::max(0, std::min(getHeight() - 1, y))];
}

#endif // __HEIGHT_MAP_H__