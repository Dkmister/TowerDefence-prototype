#ifndef __GLSL_VAO__
#define __GLSL_VAO__

#include "GLSL/Error.h"

namespace glsl {

class VertexArrayObject {
public:
   VertexArrayObject(void) {
      glGenVertexArrays(1, &_buffer);
   }

   virtual ~VertexArrayObject(void) {
      unbind();
      glDeleteVertexArrays(1,  &_buffer);
   }

   // creating and binding buffer objects
   inline void bind(void) const;
   inline void unbind(void) const;

   inline GLuint buffer(void) const;

private:
   DISALLOW_COPY_AND_ASSIGN(VertexArrayObject);
   GLuint _buffer;

};

void glsl::VertexArrayObject::bind(void) const {
   glBindVertexArray(_buffer);
}


void glsl::VertexArrayObject::unbind(void) const {
   glBindVertexArray(_buffer);
}


GLuint VertexArrayObject::buffer(void) const {
   return _buffer;
}

typedef VertexArrayObject VAO;

} // namespace glsl

#endif // __GLSL_VAO__