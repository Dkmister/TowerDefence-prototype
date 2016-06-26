#ifndef __UNIFORM_BUFFER_OBJECT_H__
#define __UNIFORM_BUFFER_OBJECT_H__

#include "glsl/Error.h"

namespace glsl {

class UniformBufferObject {
public:
   UniformBufferObject(GLenum usage) : _bufferUsage(usage) {
      glGenBuffers(1, &_buffer);
   }

   virtual ~UniformBufferObject(void) {
      glDeleteBuffers(1, &_buffer);
   }

   // creating and binding buffer objects
   inline void bind(void) const;
   inline void unbind(void) const;

   inline void bindBufferBase(GLuint index);
   inline void bindBufferRange(GLuint index, GLintptr offset, GLsizeiptr size);
   
   inline void bufferData(GLsizeiptr size, const GLvoid *data);
   inline void bufferSubData(GLintptr offset, GLsizeiptr size, const GLvoid *data);

   inline GLuint buffer(void) const;
   inline GLenum usage(void) const;

protected:
   
private:
   DISALLOW_COPY_AND_ASSIGN(UniformBufferObject);

   GLuint _buffer;
   GLenum _bufferUsage;
};

void UniformBufferObject::bind(void) const {
   glBindBuffer(GL_UNIFORM_BUFFER, _buffer);
}

void UniformBufferObject::unbind(void) const {
   glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBufferObject::bindBufferBase(GLuint index) {
   glBindBufferBase(GL_UNIFORM_BUFFER, index, _buffer);
}

void UniformBufferObject::bindBufferRange(GLuint index, GLintptr offset, GLsizeiptr size) {
   glBindBufferRange(GL_UNIFORM_BUFFER, index, _buffer, offset, size);
}

void UniformBufferObject::bufferData(GLsizeiptr size, const GLvoid *data) {
   try {
      if(glIsBuffer(_buffer) == GL_FALSE)
         throw std::runtime_error("Invalid buffer object");

      glBufferData(GL_UNIFORM_BUFFER, size, data, _bufferUsage);

      reportGLError(__FILE__, __LINE__);

   } catch(std::runtime_error &err) {
      std::cerr << "Error: Failed to create buffer object data store " << std::endl;
      std::cerr << "Reason: " << err.what () << std::endl;
   }
}

void UniformBufferObject::bufferSubData(GLintptr offset, GLsizeiptr size, const GLvoid *data) {
   try {
      if(glIsBuffer(_buffer) == GL_FALSE)
         throw std::runtime_error("Invalid buffer object");

      glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);

      reportGLError(__FILE__, __LINE__);

   } catch(std::runtime_error &err) {
      std::cerr << "Error: Failed to create buffer object data store " << std::endl;
      std::cerr << "Reason: " << err.what () << std::endl;
   }      
}

GLuint UniformBufferObject::buffer(void) const {
   return _buffer;
}

GLenum UniformBufferObject::usage(void) const {
   return _bufferUsage;
}

typedef UniformBufferObject UBO;

} // namespace glsl

#endif // __UNIFORM_BUFFER_OBJECT_H__