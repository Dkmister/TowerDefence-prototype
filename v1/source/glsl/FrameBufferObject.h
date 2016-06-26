#ifndef __GLSL_FBO_H__
#define __GLSL_FBO_H__

#include "glsl/TextureObject.h"

namespace glsl {

/*
   allows to render a scene directly to a renderbuffer object, instead of rendering to a texture object.
*/

class RenderBufferObject {
public:
   RenderBufferObject(GLsizei width, GLsizei height);

   virtual ~RenderBufferObject(void) {
      glDeleteRenderbuffers(1, &_buffer);

   }

   // renderbuffer objects
   inline void storageMultisample(GLsizei samples, GLenum internalFormat);
   inline void storage(GLenum internalFormat);

   inline void bind(void);
   inline void unbind(void);

   // renderbuffer object queries
   inline GLint getParameteriv(GLenum pname) const;

   // accessors
   inline GLuint buffer(void) const;
   inline GLuint width(void) const;
   inline GLuint height(void) const;

private:
   DISALLOW_COPY_AND_ASSIGN(RenderBufferObject);

   GLuint _buffer;
   GLsizei _width, _height;
};


void RenderBufferObject::storageMultisample(GLsizei samples, GLenum internalFormat) {
   glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, internalFormat, _width, _height);
}


void RenderBufferObject::storage(GLenum internalFormat) {
   glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, _width, _height);
}

void RenderBufferObject::bind(void) {
   glBindRenderbuffer(GL_RENDERBUFFER, _buffer);
}

void RenderBufferObject::unbind(void) {
   glBindRenderbuffer(GL_RENDERBUFFER, 0);
}


GLint RenderBufferObject::getParameteriv(GLenum pname) const {
   static GLint param;
   glGetRenderbufferParameteriv(GL_RENDERBUFFER, pname, &param);
   return param;
}

GLuint RenderBufferObject::buffer(void) const {
   return _buffer;
}

GLuint RenderBufferObject::width(void) const {
   return _width;
}

GLuint RenderBufferObject::height(void) const {
   return _height;
}

/*
   FrameBuffer Object is an extension to OpenGL for doing flexible off-screen rendering, including rendering to a texture.
*/

class FrameBufferObject {
public:
   FrameBufferObject(void) {
      glGenFramebuffers(1, &_buffer);
   }

   virtual ~FrameBufferObject(void) {
      glDeleteFramebuffers(1, &_buffer);
   }

   // binding and managing
   inline void bind(void);
   inline void unbind(void);

   // attaching texture images
   void texture(GLenum attachment, GLuint texture, GLint level) throw (std::runtime_error);

   void texture3D(GLenum attachment, const Texture3D &texture, GLint level, GLint layer) throw (std::runtime_error);
   void texture2D(GLenum attachment, const Texture2D &texture, GLint level) throw (std::runtime_error);
   void texture1D(GLenum attachment, const Texture1D &texture, GLint level) throw (std::runtime_error);

   // attaching renderbuffer images
   void renderbuffer(GLenum attachment, const RenderBufferObject &object) throw (std::runtime_error);

   // accessors
   inline GLuint buffer(void) const;

private:
   DISALLOW_COPY_AND_ASSIGN(FrameBufferObject);

   GLuint _buffer;
};

void FrameBufferObject::bind(void)  {
   glBindFramebuffer(GL_FRAMEBUFFER, _buffer);
}

void FrameBufferObject::unbind(void) {
   glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint FrameBufferObject::buffer(void) const {
   return _buffer;
}

typedef FrameBufferObject FBO;

} // namespace glsl

#endif // __GLSL_FBO_H__
