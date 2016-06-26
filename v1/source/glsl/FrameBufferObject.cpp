#include "stdafx.h"
#include "glsl/FrameBufferObject.h"
#include "glsl/Error.h"


namespace glsl {

/************************************************************************/
/* Renderbuffer Object                                                  */
/************************************************************************/

RenderBufferObject::RenderBufferObject(GLsizei width, GLsizei height) : _width(width), _height(height) {
   glGenRenderbuffers(1, &_buffer);

   try {
      if(_width > GL_MAX_RENDERBUFFER_SIZE || _height > GL_MAX_RENDERBUFFER_SIZE)
         throw("Invalid RenderBuffer object.");
   } catch(std::runtime_error &err) {
      std::cerr << "Reason: " << err.what () << std::endl;
   }
}

/************************************************************************/
/* Framebuffer Object                                                   */
/************************************************************************/

void FrameBufferObject::texture(GLenum attachment, GLuint texture, GLint level) throw (std::runtime_error) {
   glFramebufferTexture(GL_FRAMEBUFFER, attachment, texture, level);

   if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      throw("Error code: " + toString(glCheckFramebufferStatus(GL_FRAMEBUFFER)));
}

void FrameBufferObject::texture3D(GLenum attachment, const Texture3D &texture, GLint level, GLint layer) throw (std::runtime_error) {
   glFramebufferTexture3D(GL_FRAMEBUFFER, attachment, texture.target(), texture.texture(), level, layer);

   if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      throw("Error code: " + toString(glCheckFramebufferStatus(GL_FRAMEBUFFER)));
}

void FrameBufferObject::texture2D(GLenum attachment, const Texture2D &texture, GLint level) throw (std::runtime_error) {
   glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, texture.target(), texture.texture(), level);

   if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      throw("Error code: " + toString(glCheckFramebufferStatus(GL_FRAMEBUFFER)));
}

void FrameBufferObject::texture1D(GLenum attachment, const Texture1D &texture, GLint level) throw (std::runtime_error) {
   glFramebufferTexture1D(GL_FRAMEBUFFER, attachment, texture.target(), texture.texture(), level);

   if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      throw("Error code: " + toString(glCheckFramebufferStatus(GL_FRAMEBUFFER)));
}

void FrameBufferObject::renderbuffer(GLenum attachment, const RenderBufferObject &object) throw (std::runtime_error) {
   glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, object.buffer());

   if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      throw("Error code: " + toString(glCheckFramebufferStatus(GL_FRAMEBUFFER)));
}

} // namespace glsl

