#ifndef __GLSL_SHADER_H__
#define __GLSL_SHADER_H__

// based on http://tfc.duke.free.fr/

namespace glsl {

class Shader {
public:
   Shader(const std::string &fileName);
   virtual ~Shader(void);

   virtual GLenum shaderType(void) const = 0;

   GLboolean fail(void) const {
      return GL_FALSE == _compiled;
   }

   // accessors
   const std::string& fileName(void) const {
      return _fileName;
   }
   const std::string& code(void) const {
      return _code;
   }
   GLuint handle(void) const {
      return _handle;
   }

protected:
   DISALLOW_COPY_AND_ASSIGN(Shader);

   void compile(void) throw (std::runtime_error);
   void loadFile(const std::string &fileName) throw (std::runtime_error);

   std::string _fileName;
   std::string _code;
   GLuint _handle;
   GLint _compiled;
};

class VertexShader : public Shader {
public:
   VertexShader(const std::string &fileName) : Shader(fileName) {
      try {
         // load shader code from file
         loadFile(fileName);

         // compile the shader
         compile();

         std::cout << "*Vertex Shader \"" << _fileName << "\" compiled" << std::endl;

      } catch (std::runtime_error &err) {
         std::cerr << "Error: failed to create vertex shader from " << _fileName << std::endl;
         std::cerr << err.what() << std::endl;
      }
   }
   GLenum shaderType(void) const {
      return GL_VERTEX_SHADER;
   }

private:
   DISALLOW_COPY_AND_ASSIGN(VertexShader);
};

class FragmentShader : public Shader {
public:
   FragmentShader(const std::string &fileName) : Shader(fileName) {
      try {
         // load shader code from file
         loadFile(fileName);

         // compile the shader
         compile();

         std::cout << "*Fragment Shader \"" << _fileName << "\" compiled" << std::endl;

      } catch (std::runtime_error &err) {
         std::cerr << "Error: failed to create fragment shader from " << _fileName << std::endl;
         std::cerr << err.what() << std::endl;
      }
   }
   GLenum shaderType(void) const {
      return GL_FRAGMENT_SHADER;
   }

private:
   DISALLOW_COPY_AND_ASSIGN(FragmentShader);
};

class GeometryShader : public Shader {
public:
   GeometryShader(const std::string &fileName) : Shader(fileName) {
      try {
         // load shader code from file
         loadFile(fileName);

         // compile the shader
         compile();

         std::cout << "*Geometry Shader \"" << _fileName << "\" compiled" << std::endl;

      } catch (std::runtime_error &err) {
         std::cerr << "Error: failed to create geometry shader from " << _fileName << std::endl;
         std::cerr << err.what() << std::endl;
      }
   }

   GLenum shaderType(void) const {
      return GL_GEOMETRY_SHADER;
   }

private:
   DISALLOW_COPY_AND_ASSIGN(GeometryShader);
};

} // namespace glsl

#endif // __GLSL_SHADER_H__
