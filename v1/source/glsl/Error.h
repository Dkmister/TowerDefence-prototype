#ifndef __GLSL_ERROR_H__
#define __GLSL_ERROR_H__

namespace glsl {

   template<class T>
   std::string toString(T rhs)  {
      std::stringstream stream;
      stream << rhs;
      return stream.str();
   }

   template<class T>
   T fromString(const std::string &str)  {
      std::istringstream stream (str);
      T classType;
      stream >> classType;
      return classType;
   }

inline void reportGLError(const std::string& file, int line) {
   GLenum error = glGetError();

   switch(error) {
   case GL_NO_ERROR:
      break;
   case GL_INVALID_ENUM:
      throw std::runtime_error("GL_INVALID_ENUM error in File " 
         + file +  " at line: " + toString(line));
      break;
   case GL_INVALID_VALUE:
      throw std::runtime_error("GL_INVALID_VALUE error in File " 
         + file +  " at line: " + toString(line));
      break;
   case GL_INVALID_OPERATION:
      throw std::runtime_error("GL_INVALID_OPERATION error in File " 
         + file +  " at line: " + toString(line));
      break;
   case GL_INVALID_FRAMEBUFFER_OPERATION:
      throw std::runtime_error("GL_INVALID_FRAMEBUFFER_OPERATION error in File " 
         + file +  " at line: " + toString(line));
      break;
   case GL_OUT_OF_MEMORY:
      throw std::runtime_error("GL_OUT_OF_MEMORY error in File " 
         + file +  " at line: " + toString(line));
      break;
   case GL_STACK_UNDERFLOW:
      throw std::runtime_error("GL_STACK_UNDERFLOW error in File " 
         + file +  " at line: " + toString(line));
      break;
   case GL_STACK_OVERFLOW:
      throw std::runtime_error("GL_STACK_OVERFLOW error in File " 
         + file +  " at line: " + toString(line));
      break;
   default:
      throw std::runtime_error("UNKNOWN ERROR error in File " 
         + file +  " at line: " + toString(line));
   }
   return;
}

} // namespace glsl

#endif // __GLSL_ERROR_H__