#include "stdafx.h"
#include "glsl/VertexBufferObject.h"

#include "glsl/Error.h"

namespace glsl {

void VertexBufferObject::bufferData(GLsizeiptr size, const GLvoid *data) {
   try {
      bind();

      if(glIsBuffer(_buffer) == GL_FALSE)
         throw std::runtime_error("Invalid buffer object");
            
      glBufferData(_bufferTarget, size, data, _bufferUsage);
	        
      reportGLError(__FILE__, __LINE__);
      
   } catch(std::runtime_error &err) {
      std::cerr << "Error: Failed to create buffer object data store " << std::endl;
      std::cerr << "Reason: " << err.what () << std::endl;
   }
}

void VertexBufferObject::bufferSubData(GLintptr offset, GLsizeiptr size, const GLvoid *data) {
   try {
      if(glIsBuffer(_buffer) == GL_FALSE)
         throw std::runtime_error("Invalid buffer object");

      glBufferSubData(_bufferTarget, offset, size, data);

      reportGLError(__FILE__, __LINE__);

   } catch(std::runtime_error &err) {
      std::cerr << "Error: Failed to create buffer object data store " << std::endl;
      std::cerr << "Reason: " << err.what () << std::endl;
   }      
}

} // namespace glsl
