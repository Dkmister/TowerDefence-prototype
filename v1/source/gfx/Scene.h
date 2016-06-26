#ifndef __GFX_SCENE_H__
#define __GFX_SCENE_H__

#include "glsl/UniformBufferObject.h"
#include "glsl/VertexBufferObject.h"
#include "glsl/VertexArrayObject.h"

#include "glsl/TextureObject.h"

#include "glsl/ShaderProgram.h"

#include "string.h"

namespace gfx {

struct Material {
    glm::vec4 diffuse;
    glm::vec4 ambient;
    glm::vec4 specular;
    glm::vec4 emissive;
    GLfloat shininess;
    GLfloat opacity;
    GLint texCount;
};

struct Mesh {
   Mesh(void) {
      vao = NULL;

      vbo_indices   = NULL;
      vbo_positions = NULL;
      vbo_normals   = NULL;
      vbo_texCoords = NULL;

      tex = NULL;
   }

   ~Mesh(void) {
       if (vao) { delete vao; }
       if (vbo_indices  ) { delete vbo_indices;   }
       if (vbo_positions) { delete vbo_positions; }
       if (vbo_normals  ) { delete vbo_normals;   }
       if (vbo_texCoords) { delete vbo_texCoords; }

       if (tex) { delete tex; }
   }

   glsl::VAO* vao;

   glsl::VBO* vbo_indices;
   glsl::VBO* vbo_positions;
   glsl::VBO* vbo_normals;
   glsl::VBO* vbo_texCoords;

   struct Material aMat;

   glsl::Texture2D *tex;

   GLint numFaces;
};



class Scene {
public:

   Scene(util::string filename);
   ~Scene(void);

   bool load(void);
   void loadTextures(void);

   void render(glsl::ShaderProgram& program);
   void recursiveRender(glsl::ShaderProgram& program, const aiNode *node);

   inline GLfloat getScale(void) const;
   inline glm::vec3 getCenter(void) const;

private:
   util::string _filename;
   GLfloat _scale;
  
   Assimp::Importer importer;
   const aiScene* scene;

   std::vector<struct Mesh*> _meshes;
   std::map<std::string, GLuint> _textureIdMap;
};

GLfloat Scene::getScale(void) const {
   return _scale;
}

} // namespace gfx

#endif // __GFX_SCENE_H__