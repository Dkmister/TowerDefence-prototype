#include "stdafx.h"
#include "glsl/TextureObject.h"

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

#ifndef GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
#endif

#ifndef GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
#endif

#ifndef GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3
#endif

namespace glsl {

   /************************************************************************/
   /* Texture                                                              */
   /************************************************************************/

   GLint Texture::s_textureUniformValue = 0;

   /************************************************************************/
   /* Texture 3D                                                           */
   /************************************************************************/

   void Texture3D::image(GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, void *data) {
      glTexImage3D(_target, level, internalformat, width, height, depth, border, format, type, data);
   }

   void Texture3D::subImage(GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, void *data) {
      glTexSubImage3D(_target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, data);
   }

   void Texture3D::copySubImage(GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height) {
      glCopyTexSubImage3D(_target, level, xoffset, yoffset, zoffset, x, y, width, height);
   }

   bool Texture3D::load(std::string file) {
      return false;
   }

   /************************************************************************/
   /* Texture 2D                                                           */
   /************************************************************************/

   void Texture2D::image(GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, void *data) {
      glTexImage2D(_target, level, internalformat, width, height, border, format, type, data);
   }

   void Texture2D::copyImage(GLint level, GLint internalformat, GLint x, GLint y, GLint width, GLsizei height, GLint border) {
      glCopyTexImage2D(_target, level, internalformat, x, y, width, height, border);
   }

   void Texture2D::subImage(GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, void *data) {
      glTexSubImage2D(_target, level, xoffset, yoffset, width, height, format, type, data);
   }

   void Texture2D::copySubImage(GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height) {
      glCopyTexSubImage2D(_target, level, xoffset, yoffset, x, y, width, height);
   }

   bool Texture2D::load(std::string file) {
      // load image
      FREE_IMAGE_FORMAT format = FreeImage_GetFileType(file.c_str(), 0);
      FIBITMAP* unconvertedData = FreeImage_Load(format, file.c_str());

      if (unconvertedData == NULL) {
         std::cerr << "Texture::load(): Unable to load \"" << file << "\".\n";
         return false;
      }

      FIBITMAP * data = FreeImage_ConvertTo32Bits(unconvertedData);
      FreeImage_Unload(unconvertedData);

      bind();
      {
         image(0, GL_RGB, FreeImage_GetWidth(data), FreeImage_GetHeight(data),
            0, GL_BGRA, GL_UNSIGNED_BYTE, FreeImage_GetBits (data));

         parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
         parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

         parameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
         parameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

         glGenerateMipmap(GL_TEXTURE_2D);
      }
      unbind();

      return true;
   }

   bool Texture2D::loadDDS(std::string file) {

      unsigned char header[124];

      FILE *fp; 

      /* try to open the file */ 
      fp = fopen(file.c_str(), "rb"); 
      if (fp == NULL){
         printf("%s could not be opened.\n", file.c_str()); getchar(); 
         return false;
      }

      /* verify the type of file */ 
      char filecode[4]; 
      fread(filecode, 1, 4, fp); 
      if (strncmp(filecode, "DDS ", 4) != 0) { 
         fclose(fp); 
         return false; 
      }

      /* get the surface desc */ 
      fread(&header, 124, 1, fp); 

      unsigned int height      = *(unsigned int*)&(header[8 ]);
      unsigned int width	     = *(unsigned int*)&(header[12]);
      unsigned int linearSize	 = *(unsigned int*)&(header[16]);
      unsigned int mipMapCount = *(unsigned int*)&(header[24]);
      unsigned int fourCC      = *(unsigned int*)&(header[80]);


      unsigned char * buffer;
      unsigned int bufsize;
      /* how big is it going to be including all mipmaps? */ 
      bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize; 
      buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char)); 
      fread(buffer, 1, bufsize, fp); 
      /* close the file pointer */ 
      fclose(fp);

      unsigned int components  = (fourCC == FOURCC_DXT1) ? 3 : 4; 
      unsigned int format;
      switch(fourCC) 
      { 
      case FOURCC_DXT1: 
         format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; 
         break; 
      case FOURCC_DXT3: 
         format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; 
         break; 
      case FOURCC_DXT5: 
         format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; 
         break; 
      default: 
         free(buffer); 
         return false; 
      }

      // "Bind" the newly created texture : all future texture functions will modify this texture
      glBindTexture(GL_TEXTURE_2D, _texture);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);	

      unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16; 
      unsigned int offset = 0;

      /* load or generate the mipmaps */ 
      if (mipMapCount) {
         for (unsigned int level = 0; level < mipMapCount && (width || height); ++level) { 
            unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize; 
            glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,  
               0, size, buffer + offset); 

            offset += size; 
            width  /= 2; 
            height /= 2; 

            // Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
            if(width < 1) width = 1;
            if(height < 1) height = 1;

         }
      } else {
         glGenerateMipmap(GL_TEXTURE_2D);
      }    

      parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      parameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
      parameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

      free(buffer); 

      return true;
   }

   /************************************************************************/
   /* Texture 1D                                                           */
   /************************************************************************/

   void Texture1D::image(GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, void *data) {
      glTexImage1D(_target, level, internalformat, width, border, format, type, data);
   }

   void Texture1D::copyImage(GLint level, GLint internalformat, GLint x, GLint y, GLint width, GLint border) {
      glCopyTexImage1D(_target, level, internalformat, x, y, width, border);
   }

   void Texture1D::subImage(GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, void *data) {
      glTexSubImage1D(_target, level, xoffset, width, format, type, data);
   }

   void Texture1D::copySubImage(GLint level, GLint xoffset, GLint x, GLint y, GLsizei width) {
      glCopyTexSubImage1D(_target, level, xoffset, x, y, width);
   }

   bool Texture1D::load(std::string file) {
      return false;
   }

} // namespace glsl
