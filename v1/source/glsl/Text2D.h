#ifndef __GLSL_TEXT_2D_H__
#define __GLSL_TEXT_2D_H__

#include "glsl/ShaderProgram.h"
#include "glsl/TextureObject.h"

#include "glsl/VertexArrayObject.h"
#include "glsl/VertexBufferObject.h"

namespace glsl {

class Text2D {
public:
	Text2D(const std::string& texture);
	~Text2D();

    void render(const std::string& text, float x, float y, float size);

protected:
    VAO vao;
    VBO vbo_vertex, vbo_uv;
    
    Texture2D texture;
    ShaderProgram program;    
};

} // namespace glsl

#endif // __GLSL_TEXT_2D_H__
