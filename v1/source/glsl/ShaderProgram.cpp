#include "stdafx.h"
#include "glsl/ShaderProgram.h"

namespace glsl {

ShaderProgram::ShaderProgram(const std::string &name, const VertexShader& vertShader, const FragmentShader& fragShader)
      : _name(name), _handle(0), _linked(0) {
   try {
      if (vertShader.fail())
         throw std::runtime_error ("Invalid vertex shader");

      if (fragShader.fail())
         throw std::runtime_error ("Invalid fragment shader");

      // create program and attach
      _handle = glCreateProgram();

      // vertex shader
      glAttachShader(_handle, vertShader.handle());
      // fragment shadero
      glAttachShader(_handle, fragShader.handle());

      // perform link stage
      glLinkProgram(_handle);
      glGetProgramiv(_handle, GL_LINK_STATUS, &_linked);

      // validate program
      glValidateProgram(_handle);

      // Check for success
      if (GL_FALSE == _linked)
         throw std::runtime_error ("Link stage failed");

   } catch(std::runtime_error &err) {
      std::cerr << "Error: Failed to create program shader " << _name << std::endl;
      std::cerr << err.what() << "\n" << std::endl;
      exit(-1);
   }
}

ShaderProgram::ShaderProgram(const std::string &name, const VertexShader& vertShader, const GeometryShader& geomShader, const FragmentShader& fragShader)
      : _name(name), _handle(0), _linked(0) {
   try {
      if (vertShader.fail())
         throw std::runtime_error ("Invalid vertex shader");

      if (geomShader.fail())
         throw std::runtime_error ("Invalid geometry shader");

      if (fragShader.fail())
         throw std::runtime_error ("Invalid fragment shader");

      // create program and attach
      _handle = glCreateProgram();

      // vertex shader
      glAttachShader(_handle, vertShader.handle());
      // fragment shader
      glAttachShader(_handle, fragShader.handle());
      // geometry shader
      glAttachShader(_handle, geomShader.handle());

      // perform link stage
      glLinkProgram(_handle);
      glGetProgramiv(_handle, GL_LINK_STATUS, &_linked);

      // validate program
      glValidateProgram(_handle);

      // Check for success
      if (GL_FALSE == _linked)
         throw std::runtime_error ("Link stage failed");

   } catch(std::runtime_error &err) {
      std::cerr << "Error: Failed to create program shader " << _name << std::endl;
      std::cerr << err.what() << "\n" << std::endl;
      exit(-1);
   }
}

void ShaderProgram::activeTexture(const std::string& sampler, const Texture &texture) {
   activeTexture(getUniformLocation(sampler), texture);
}

void ShaderProgram::activeTexture(GLint textureLocation, const Texture &texture) {
   if (_textures.find(textureLocation) == _textures.end())
      _textures[textureLocation] = _textures.size();

   glUniform1i(textureLocation, _textures[textureLocation]);
   glActiveTexture(GL_TEXTURE0 + _textures[textureLocation]);
   texture.bind();
}

GLint ShaderProgram::getUniformLocation(const std::string &name) {
   if (_uniforms.find(name) == _uniforms.end())
      _uniforms[name] = glGetUniformLocation(_handle, name.c_str());

   return _uniforms[name];
}

GLint ShaderProgram::getAttribLocation(const std::string &name) {
   if (_attribs.find(name) == _attribs.end())
      _attribs[name] = glGetAttribLocation(_handle, name.c_str());

   return _attribs[name];
}

void ShaderProgram::bindAttribLocation(GLuint index, const std::string &name) {
   if (_attribs.find(name) == _attribs.end())
      _attribs[name] = index;

   glBindAttribLocation(_handle, _attribs[name], name.c_str());
}

} // namespace glsl

