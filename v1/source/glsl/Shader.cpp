#include "stdafx.h"
#include "glsl/Shader.h"

namespace glsl {
   
Shader::Shader(const std::string &fileName) : _fileName(fileName), _handle(0), _compiled(0) {
   // none
}

Shader::~Shader(void) {
   if (glIsShader(_handle)) {
      glDeleteShader(_handle);
   }
}

void Shader::compile(void) throw (std::runtime_error) {
   const GLchar *code = _code.c_str();

   // create a shader object
   _handle = glCreateShader(shaderType());

   // upload shader code to OpenGL
   glShaderSource(_handle, 1, &code, NULL);

   // compile shader
   glCompileShader(_handle);
   glGetShaderiv(_handle, GL_COMPILE_STATUS, &_compiled);

   // check for success
   if (GL_FALSE == _compiled) {
      GLint length = 0;
      std::string infoLog;

      glGetShaderiv(_handle, GL_INFO_LOG_LENGTH, &length);

      if (length > 0) {
         GLint charsWritten = 0;
         infoLog.resize(length + 1);
         glGetShaderInfoLog(_handle, length, &charsWritten, &infoLog[0]);
      }

      throw std::runtime_error(infoLog);
   }
}

void Shader::loadFile(const std::string &fileName) throw (std::runtime_error) {
   // open file
   std::ifstream ifs(fileName.c_str(), std::ios::in | std::ios::binary);

   if (ifs.fail())
      throw std::runtime_error ("Couldn't open file");

   // read whole file into string
   _code.assign (std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());

   // close file
   ifs.close ();
}

} // namespace glsl