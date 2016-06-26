#ifndef __GLSL_SHADER_PROGRAM_H__
#define __GLSL_SHADER_PROGRAM_H__

// based on http://tfc.duke.free.fr/

#include "glsl/Shader.h"
#include "glsl/TextureObject.h"

namespace glsl {

class ShaderProgram {
public:
    ShaderProgram() { };

   ShaderProgram(const std::string &name, const VertexShader& vertShader, const FragmentShader& fragShader);
   ShaderProgram(const std::string &name, const VertexShader& vertShader, const GeometryShader& geomShader, const FragmentShader& fragShader);
   virtual ~ShaderProgram(void) {
      unuse();
      glDeleteProgram (_handle);
   }

   // manage shader textures
   void activeTexture(const std::string& sampler, const Texture &texture);
   void activeTexture(GLint textureLocation, const Texture &texture);

   // fragment shaders
   void bindFragDataLocation(int colorNumber, const std::string &name) {
      glBindFragDataLocation(_handle, colorNumber, name.c_str());
   }
   GLint getFragDataLocation(const std::string &name) {
      return glGetFragDataLocation(_handle, name.c_str());
   }

   GLuint getUniformBlockIndex(const std::string &uniformBlockName) {
      return glGetUniformBlockIndex(_handle, uniformBlockName.c_str());
   }
   void uniformBlockBinding(GLuint uniformBlockIndex, GLuint uniformBlockBinding) {
      glUniformBlockBinding(_handle, uniformBlockIndex, uniformBlockBinding);
   }

   // uniform variables
   GLint getUniformLocation(const std::string &name);

   // vertex attributes
   GLint getAttribLocation(const std::string &name);
   void bindAttribLocation(GLuint index, const std::string &name);

   // program objects
   inline void use(void) const;
   inline void unuse(void) const;

   // accessors
   GLuint program(void) const {
      return _handle;
   }

private:
   //DISALLOW_COPY_AND_ASSIGN(ShaderProgram);

   std::map<GLint, GLint> _textures;
   std::map<std::string, GLint> _uniforms;
   std::map<std::string, GLint> _attribs;

   std::string _name;
   GLuint _handle;
   GLint _linked;
};

void ShaderProgram::use(void) const {
   glUseProgram(_handle);
}

void ShaderProgram::unuse(void) const {
   glUseProgram(0);
}

} // namespace glsl

#endif // __GLSL_SHADER_PROGRAM_H__
