#ifndef __GLSL_TEXTURE_H__
#define __GLSL_TEXTURE_H__

namespace glsl {

class Texture {
public:
   inline void bind(void) const;
   inline void unbind(void) const;

   // texture parameters
   inline void parameter(GLenum pname, GLfloat param);
   inline void parameter(GLenum pname, GLint param);

   // texture minification
   inline void generateMipmap(void) const;


   void getImage(GLint lod, GLenum format, GLenum type, void *img) {
      glGetTexImage(_texture, lod, format, type, img);
   }

   // accessors
   inline GLuint texture(void) const;
   inline GLenum target(void) const;
   inline GLint unifirmValue(void) const;

   virtual bool load(std::string file) = 0;

protected:
   Texture(GLenum target) : _target(target), _uniformValue(s_textureUniformValue++) {
      glGenTextures(1, &_texture);
   }

   virtual ~Texture(void) {
      unbind();
      glDeleteTextures(1, &_texture);
   }

protected:
   const  GLint _uniformValue;
   GLuint _texture;
   GLenum _target;

private:
   static GLint s_textureUniformValue;

   DISALLOW_COPY_AND_ASSIGN(Texture);
};

void Texture::bind(void) const {
   //glEnable(_target);
   glBindTexture(_target, _texture);
}

void Texture::unbind(void) const {
   glBindTexture(_target, 0);
   //glDisable(_target);
}

void Texture::parameter(GLenum pname, GLfloat param) {
   glTexParameterf(_target, pname, param);
}

void Texture::parameter(GLenum pname, GLint param) {
   glTexParameteri(_target, pname, param);
}

void Texture::generateMipmap(void) const {
   glGenerateMipmap(_target);
}

GLuint Texture::texture(void) const {
   return _texture;
}

GLenum Texture::target(void) const {
   return _target;
}

GLint Texture::unifirmValue(void) const {
   return _uniformValue;
}

/************************************************************************/
/* Texture 3D                                                           */
/************************************************************************/

class Texture3D : public Texture {
public:
   enum textarget {
      TEXTURE_3D = GL_TEXTURE_3D,
      TEXTURE_2D_ARRAY = GL_TEXTURE_2D_ARRAY,
      TEXTURE_CUBE_MAP_ARRAY = GL_TEXTURE_CUBE_MAP_ARRAY
   };

   Texture3D(const Texture3D::textarget &target) : Texture(target) {
      // none
   }

   // texture image specification
   void image(GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, void *data);

   // alternative texture image specification
   void subImage(GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, void *data);
   void copySubImage(GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);

   bool load(std::string file);

private:
   DISALLOW_COPY_AND_ASSIGN(Texture3D);
};

/************************************************************************/
/* Texture 2D                                                           */
/************************************************************************/

class Texture2D : public Texture {
public:
   enum textarget {
      TEXTURE_2D = GL_TEXTURE_2D,
      TEXTURE_RECTANGLE = GL_TEXTURE_RECTANGLE,
      TEXTURE_1D_ARRAY = GL_TEXTURE_1D_ARRAY,

      TEXTURE_CUBE_MAP_POSITIVE_X = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
      TEXTURE_CUBE_MAP_POSITIVE_Y = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
      TEXTURE_CUBE_MAP_POSITIVE_Z = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,

      TEXTURE_CUBE_MAP_NEGATIVE_X = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
      TEXTURE_CUBE_MAP_NEGATIVE_Y = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
      TEXTURE_CUBE_MAP_NEGATIVE_Z = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
   };

   Texture2D(const Texture2D::textarget &target) : Texture(target) {
      // none
   }

   // texture image specification
   void image(GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, void *data);

   // alternative texture image specification
   void copyImage(GLint level, GLint internalformat, GLint x, GLint y, GLint width, GLsizei height, GLint border);
   void subImage(GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, void *data);
   void copySubImage(GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);

   bool load(std::string file);
   bool loadDDS(std::string file);

private:
   DISALLOW_COPY_AND_ASSIGN(Texture2D);
};

/************************************************************************/
/* Texture 1D                                                           */
/************************************************************************/

class Texture1D : public Texture {
public:
   Texture1D(void) : Texture(GL_TEXTURE_1D) {
      // none
   }

   // texture image specification
   void image(GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, void *data);

   // alternative texture image specification
   void copyImage(GLint level, GLint internalformat, GLint x, GLint y, GLint width, GLint border);
   void subImage(GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, void *data);
   void copySubImage(GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);

   bool load(std::string file);

private:
   DISALLOW_COPY_AND_ASSIGN(Texture1D);
};

} // namespace glsl

#endif // __GLSL_TEXTURE_H__
