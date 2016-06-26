#include "stdafx.h"

#include "Matrices.h"
#include "DemoScene.h"

DemoScene::DemoScene() {

   GLuint vertexLocation = 0, uvLocation = 1;

   // Create and compile our GLSL program from the shaders
   _shader.reset(new glsl::ShaderProgram("default_shader",
      glsl::VertexShader("./resources/shaders/default_shader.vert"), 
      glsl::FragmentShader("./resources/shaders/default_shader.frag")));

   _shader->bindAttribLocation(vertexLocation, "in_vertex");
   _shader->bindAttribLocation(uvLocation, "in_texcoord");

   //////////////////////////////////////////////////////////////////////////////////////

   // Load the texture   
   _texture = loadBMP("./resources/scenes/cube/default.bmp");
   //_texture = loadBMP("./resources/scenes/cube/texture.bmp");

   std::vector<glm::vec3> vertices, normals;
   std::vector<glm::vec2> uvs;

   // Read our .obj file
   bool res = loadOBJ("./resources/scenes/cube/cube.obj", vertices, uvs, normals);
   _size = vertices.size();

   //////////////////////////////////////////////////////////////////////////////////////

   _vao.reset(new glsl::VertexArrayObject());
   _vao->bind();

   _vbo_data.reset(new glsl::VertexBufferObject(GL_ARRAY_BUFFER, GL_STATIC_DRAW));
   _vbo_data->bufferData(vertices.size() * sizeof(glm::vec3), &vertices[0]);

   _vbo_data->bind();
   // 1rst attribute buffer : vertices
   glEnableVertexAttribArray(vertexLocation);
   glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

   _vbo_uv.reset(new glsl::VertexBufferObject(GL_ARRAY_BUFFER, GL_STATIC_DRAW));
   _vbo_uv->bufferData(uvs.size() * sizeof(glm::vec2), &uvs[0]);   

   _vbo_uv->bind();
   // 2nd attribute buffer : UVs
   glEnableVertexAttribArray(uvLocation);
   glVertexAttribPointer(uvLocation, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

   //////////////////////////////////////////////////////////////////////////////////////

   camera.reset(new ArcballCamera(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.0f, 1.0f, 0.0f)));
}

DemoScene::~DemoScene() {
   glDeleteTextures(1, &_texture);
}

void DemoScene::render(GLFWwindow* window) {
   camera->computeFromInputs(window);

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glCullFace(GL_BACK);

   int width, height;
   glfwGetWindowSize(window, &width, &height);

   // Use our shader
   _shader->use();

   // Compute the MVP matrix
   glm::mat4 Projection = glm::perspective(60.f, width / (float)height, 0.1f, 100.0f);
   glm::mat4 View = camera->getMatrix();
   glm::mat4 Model = glm::mat4(1.0);

   util::updateMVP(*_shader, Model, View, Projection);

   _vao->bind();

   // Bind our texture in Texture Unit 0
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, _texture);
   glUniform1i(glGetUniformLocation(_shader->program(), "myTextureSampler"), 0);

   // Draw the triangle
   glDrawArrays(GL_TRIANGLES, 0, _size);

   _vao->unbind();
}
