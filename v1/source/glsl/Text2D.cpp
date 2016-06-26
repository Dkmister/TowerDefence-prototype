#include "stdafx.h"
#include "glsl/Text2D.h"
#include "glsl/Error.h"

glsl::Text2D::Text2D(const std::string& texture_file) 
        : texture(Texture2D::TEXTURE_2D), 
        program("texture_shader", VertexShader("./resources/texture.vert"), FragmentShader("./resources/texture.frag")),
        vbo_vertex(GL_ARRAY_BUFFER, GL_STATIC_DRAW),
        vbo_uv(GL_ARRAY_BUFFER, GL_STATIC_DRAW) {
    // Initialize texture
    texture.loadDDS(texture_file);

    vao.bind();
    // 1rst attribute buffer : vertices
    vbo_vertex.bind();
    glEnableVertexAttribArray(0);    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

    // 2nd attribute buffer : UVs
    vbo_uv.bind();
    glEnableVertexAttribArray(1);    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

    vao.unbind();
}

glsl::Text2D::~Text2D() { }

void glsl::Text2D::render(const std::string& text, float x, float y, float size) {
    unsigned int length = strlen(text.c_str());

    // Fill buffers
    std::vector<glm::vec2> vertices;
    std::vector<glm::vec2> UVs;
    for ( unsigned int i=0 ; i<length ; i++ ){

        glm::vec2 vertex_up_left    = glm::vec2( x+i*size     , y+size );
        glm::vec2 vertex_up_right   = glm::vec2( x+i*size+size, y+size );
        glm::vec2 vertex_down_right = glm::vec2( x+i*size+size, y      );
        glm::vec2 vertex_down_left  = glm::vec2( x+i*size     , y      );

        vertices.push_back(vertex_up_left   );
        vertices.push_back(vertex_down_left );
        vertices.push_back(vertex_up_right  );

        vertices.push_back(vertex_down_right);
        vertices.push_back(vertex_up_right);
        vertices.push_back(vertex_down_left);

        char character = text[i];
        float uv_x = (character%16)/16.0f;
        float uv_y = (character/16)/16.0f;

        glm::vec2 uv_up_left    = glm::vec2( uv_x           , uv_y );
        glm::vec2 uv_up_right   = glm::vec2( uv_x+1.0f/16.0f, uv_y );
        glm::vec2 uv_down_right = glm::vec2( uv_x+1.0f/16.0f, (uv_y + 1.0f/16.0f) );
        glm::vec2 uv_down_left  = glm::vec2( uv_x           , (uv_y + 1.0f/16.0f) );
        UVs.push_back(uv_up_left   );
        UVs.push_back(uv_down_left );
        UVs.push_back(uv_up_right  );

        UVs.push_back(uv_down_right);
        UVs.push_back(uv_up_right);
        UVs.push_back(uv_down_left);
    }

    
    vbo_vertex.bind();
    vbo_vertex.bufferData(vertices.size() * sizeof(glm::vec2), &vertices[0]);
    vbo_vertex.unbind();

    vbo_uv.bind();
    vbo_uv.bufferData(UVs.size() * sizeof(glm::vec2), &UVs[0]);
    vbo_uv.unbind();    

    program.use();
    program.activeTexture(0, texture);
        
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Draw call
    vao.bind();    
    glDrawArrays(GL_TRIANGLES, 0, vertices.size() );
    vao.unbind();

    glDisable(GL_BLEND);

    program.unuse();
}
