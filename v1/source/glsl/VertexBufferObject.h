#ifndef __GLSL_VBO_H__
#define __GLSL_VBO_H__

namespace glsl {

class VertexBufferObject {
public:
   VertexBufferObject(GLenum target, GLenum usage) : _bufferTarget(target), _bufferUsage(usage) {
      glGenBuffers(1, &_buffer);
   }

   virtual ~VertexBufferObject(void) {
      unbind();
      glDeleteBuffers(1, &_buffer);
   }
      
   // creating and binding buffer objects
   inline void bind(void) const;
   inline void unbind(void) const;

   // creating buffer object data stores
   void bufferData(GLsizeiptr size, const GLvoid *data);
   void bufferSubData(GLintptr offset, GLsizeiptr size, const GLvoid *data);

   // accessors
   inline GLuint buffer(void) const;
   inline GLenum target(void) const;
   inline GLenum usage(void) const;

private:
   DISALLOW_COPY_AND_ASSIGN(VertexBufferObject);

   GLuint _buffer;
   GLenum _bufferTarget;
   GLenum _bufferUsage;
};

void VertexBufferObject::bind(void) const {
   glBindBuffer(_bufferTarget, _buffer);
}

void VertexBufferObject::unbind(void) const {
   glBindBuffer(_bufferTarget, 0);
}

GLuint VertexBufferObject::buffer(void) const {
   return _buffer;
}

GLenum VertexBufferObject::target(void) const {
   return _bufferTarget;
}

GLenum VertexBufferObject::usage(void) const {
   return _bufferUsage;
}

typedef VertexBufferObject VBO;

} // namespace glsl

#endif // __GLSL_VBO_H__