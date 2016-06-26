#include "stdafx.h"
#include "HeightMap.h"

#include "Matrices.h"
#include "gfx/ogldev_math_3d.h"

HeightMap::HeightMap(const std::string path) : _loaded(false) {
   _path = path;
   _scale = glm::vec3(1000.0f, 50.0f, 1000.0f);

   _vbos.resize(3, nullptr);
   _vaos.resize(2, nullptr);

   _programs.resize(2, nullptr);

   _programs[0] = new glsl::ShaderProgram(
      "HeightMap",
      glsl::VertexShader("./resources/heightmap/terrain.vert"),
      glsl::FragmentShader("./resources/heightmap/terrain.frag")
      );

   _programs[1] = new glsl::ShaderProgram(
      "HeightMapGrass",
      glsl::VertexShader("./resources/heightmap/grass.vert"),
      glsl::GeometryShader("./resources/heightmap/grass.geom"),
      glsl::FragmentShader("./resources/heightmap/grass.frag")
   );

   //////////////////////////////////////////////////////////////////////////////////////

   glsl::Texture2D* texture = nullptr;

   texture = new glsl::Texture2D(glsl::Texture2D::TEXTURE_2D);
   texture->loadDDS("./resources/textures/fungus.dds");
   _textures.push_back(texture);

   texture = new glsl::Texture2D(glsl::Texture2D::TEXTURE_2D);
   texture->loadDDS("./resources/textures/sand_grass_02.DDS");
   _textures.push_back(texture);

   texture = new glsl::Texture2D(glsl::Texture2D::TEXTURE_2D);
   texture->loadDDS("./resources/textures/rock_2_4w.DDS");
   _textures.push_back(texture);

   texture = new glsl::Texture2D(glsl::Texture2D::TEXTURE_2D);
   texture->loadDDS("./resources/textures/sand.DDS");
   _textures.push_back(texture);

   texture = new glsl::Texture2D(glsl::Texture2D::TEXTURE_2D);
   texture->load("./resources/textures/path.png");
   _textures.push_back(texture);

   texture = new glsl::Texture2D(glsl::Texture2D::TEXTURE_2D);
   texture->loadDDS("./resources/textures/sidewalk.DDS");
   _textures.push_back(texture);

   texture = new glsl::Texture2D(glsl::Texture2D::TEXTURE_2D);
   texture->loadDDS("./resources/textures/grassPack.dds");
   _textures.push_back(texture);
}

HeightMap::~HeightMap() {
   release();
}

bool HeightMap::load() {
   if(_loaded) {
      _loaded = false;
      release();
   }

   FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
   FIBITMAP* dib(0);

   // Check the file signature and deduce its format
   fif = FreeImage_GetFileType(_path.c_str(), 0);

   // If still unknown, try to guess the file format from the file extension
   if(fif == FIF_UNKNOWN)
      fif = FreeImage_GetFIFFromFilename(_path.c_str());

   // If still unknown, return failure
   if(fif == FIF_UNKNOWN)
      return false;

   // Check if the plugin has reading capabilities and load the file
   if(FreeImage_FIFSupportsReading(fif))
      dib = FreeImage_Load(fif, _path.c_str());

   if(!dib) return false;

   // Retrieve the image data
   _height = FreeImage_GetHeight(dib);
   _width = FreeImage_GetWidth(dib);

   // We also require our image to be 8-bit (luminance)
   if(_height == 0 || _width == 0 || FreeImage_GetBPP(dib) != 8)
      return false;

   //////////////////////////////////////////////////////////////////////////////////////

   vertex_data.resize(_height * _width);
   std::vector<glm::vec2> coord_data(_height * _width);
   std::vector<glm::vec3> normal_data(_height * _width);

   // All vertex data are here (there are iRows*iCols vertices in this heightmap), we will get to normals later
   float fTextureU = float(_width) * 0.1f;
   float fTextureV = float(_height) * 0.1f;

   for(int i = 0; i < _height; ++i) {
      for(int j = 0; j < _width; ++j) {
         float fScaleC = float(j)/float(_width-1);
         float fScaleR = float(i)/float(_height-1);

         BYTE value;
         FreeImage_GetPixelIndex(dib, j, i, &value);

         float fVertexHeight = ((float)value) / 255.0f;

         vertex_data[i * _width + j] = glm::vec3((-0.5f+fScaleC) * _scale.x, (fVertexHeight) * _scale.y, (-0.5f+fScaleR) * _scale.z);
         coord_data[i * _width + j] = glm::vec2(fTextureU*fScaleC, 1.f - (fTextureV*fScaleR));
      }
   }

   // Normals are here - the heightmap contains ( (iRows-1)*(iCols-1) quads, each one containing 2 triangles, therefore array of we have 3D array)
   std::vector<std::vector<glm::vec3>> vNormals[2];
   vNormals[0] = std::vector<std::vector<glm::vec3>>(_height-1, std::vector<glm::vec3>(_width-1));
   vNormals[1] = std::vector<std::vector<glm::vec3>>(_height-1, std::vector<glm::vec3>(_width-1));

   for(int i = 0; i < _height - 1; ++i) {
      for(int j = 0; j < _width - 1; ++j) {
         glm::vec3 vTriangle0[] = 
         {
            vertex_data[i * _width + j],
            vertex_data[(i + 1) * _width + j],
            vertex_data[(i + 1) * _width + (j + 1)],
         };
         glm::vec3 vTriangle1[] = 
         {
            vertex_data[(i + 1) * _width + j],
            vertex_data[i * _width + (j + 1)],
            vertex_data[i * _width + j],
         };

         glm::vec3 vTriangleNorm0 = glm::cross(vTriangle0[0]-vTriangle0[1], vTriangle0[1]-vTriangle0[2]);
         glm::vec3 vTriangleNorm1 = glm::cross(vTriangle1[0]-vTriangle1[1], vTriangle1[1]-vTriangle1[2]);

         vNormals[0][i][j] = glm::normalize(vTriangleNorm0);
         vNormals[1][i][j] = glm::normalize(vTriangleNorm1);
      }
   }

   for(int i = 0; i < _height; ++i) {
      for(int j = 0; j < _width; ++j) {
         // Now we wanna calculate final normal for [i][j] vertex. We will have a look at all triangles this vertex is part of, and then we will make average vector
         // of all adjacent triangles' normals

         glm::vec3 vFinalNormal = glm::vec3(0.0f, 0.0f, 0.0f);

         // Look for upper-left triangles
         if(j != 0 && i != 0) {
            vFinalNormal += vNormals[0][i-1][j-1];
            vFinalNormal += vNormals[1][i-1][j-1];
         }
            
         // Look for upper-right triangles
         if(i != 0 && j != _width-1) {
            vFinalNormal += vNormals[0][i-1][j];
         }

         // Look for bottom-right triangles
         if(i != _height-1 && j != _width-1) {
            vFinalNormal += vNormals[0][i][j];
            vFinalNormal += vNormals[1][i][j];
         }

         // Look for bottom-left triangles
         if(i != _height-1 && j != 0) {
            vFinalNormal += vNormals[1][i][j-1];
         }

         vFinalNormal = glm::normalize(vFinalNormal);

         // Store final normal of j-th vertex in i-th row
         normal_data[i * _width + j] = vFinalNormal;
      }
   }

   //////////////////////////////////////////////////////////////////////////////////////

   // Create a VBO with only vertex data
   _vbos[0] = new glsl::VBO(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
   _vbos[0]->bufferData((sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3)) * (_height * _width), nullptr);

   _vbos[0]->bufferSubData(0, sizeof(glm::vec3) * (_height * _width), &vertex_data[0]);
   _vbos[0]->bufferSubData(sizeof(glm::vec3) * (_height * _width), sizeof(glm::vec2) * (_height * _width), &coord_data[0]);
   _vbos[0]->bufferSubData((sizeof(glm::vec3) + sizeof(glm::vec2)) * (_height * _width), sizeof(glm::vec3) * (_height * _width), &normal_data[0]);

   // Now create a VBO with heightmap indices
   std::vector<unsigned int> vbo_indices_vector;

   int iPrimitiveRestartIndex = _height * _width;
   for(int i = 0; i < _height - 1; ++i) {      
      for(int j = 0; j < _width; ++j) {

         int iRow, iIndex;

         iRow = i;
         iIndex = iRow * _width + j;
         vbo_indices_vector.push_back(iIndex);

         iRow = i + 1;
         iIndex = iRow * _width + j;
         vbo_indices_vector.push_back(iIndex);
      }
      
      // Restart triangle strips
      vbo_indices_vector.push_back(iPrimitiveRestartIndex);
   }
   _vbos[1] = new glsl::VBO(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
   _vbos[1]->bufferData(sizeof(unsigned int) * vbo_indices_vector.size(), &vbo_indices_vector[0]);


   _vaos[0] = new glsl::VAO();
   _vaos[0]->bind();

   // Attach vertex data to this VAO
   _vbos[0]->bind();

   // Vertex positions
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
   // Texture coordinates
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)((sizeof(glm::vec3)) * (_width * _height)));
   // Normal vectors
   glEnableVertexAttribArray(2);
   glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)((sizeof(glm::vec3) + sizeof(glm::vec2)) * (_width * _height)));

   // And now attach index data to this VAO
   // Here don't forget to bind another type of VBO - the element array buffer
   _vbos[1]->bind();

   _vaos[0]->unbind();

   //////////////////////////////////////////////////////////////////////////////////////
      
   const int iGrassPerPatch = 1;         // [0, MAX_INT]
   const float fGrassDensity = 1.0;      // [0.0, 1.0]
   const float fGrassPatchOffset = 2.0f; // [0.0, MAX_FLOAT]

   _numGrassTrinagles = 0;

   glm::vec3 vCurPatchPos(0.0f);

   for (int y = 2; y < _height - 2; ++y) {
      for (int x = 2; x < _width - 2; ++x) {
         for (int i = 0; i < iGrassPerPatch; ++i) {
            // density
            if (fGrassDensity >= RandomFloat(0.f, 1.f)) {            
               vCurPatchPos.x = vertex_data[y * _width + x].x + (RandomFloat(-1.f, 1.f) * fGrassPatchOffset);
               vCurPatchPos.y = vertex_data[y * _width + x].y - 0.3f;
               vCurPatchPos.z = vertex_data[y * _width + x].z + (RandomFloat(-1.f, 1.f) * fGrassPatchOffset);

               vbo_grass_data.push_back(vCurPatchPos);

               _numGrassTrinagles += 1;
            }
         }         
      }
   }

   _vaos[1] = new glsl::VAO();
   _vaos[1]->bind();

   _vbos[2] = new glsl::VBO(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
   _vbos[2]->bufferData(sizeof(glm::vec3) * vbo_grass_data.size(), &vbo_grass_data[0]);

   // Vertex positions
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

   _vaos[1]->unbind();

   //////////////////////////////////////////////////////////////////////////////////////
   
   _loaded = true;
   return _loaded;
}

void HeightMap::release() {
   for (size_t i = 0; i < _programs.size(); ++i) {
      if (_programs[i])
         delete _programs[i];
   }
   _programs.clear();

   for (size_t i = 0; i < _vbos.size(); ++i) {
      if (_vbos[i])
         delete _vbos[i];
   }
   _vbos.clear();

   for (size_t i = 0; i < _vaos.size(); ++i) {
      if (_vaos[i])
         delete _vaos[i];
   }
   _vaos.clear();

   for (size_t i = 0; i < _textures.size(); ++i) {
      if (_textures[i])
         delete _textures[i];
   }
   _textures.clear();
}

void HeightMap::render(const Camera& camera, const glm::mat4& projection, bool grass) {
   if(!_loaded) return;

   /*// sorting
   if (sort_depth.sort(camera.getEye())) {
      std::sort(vbo_grass_data.begin(), vbo_grass_data.end(), sort_depth);
      _vbos[2]->bufferData(sizeof(glm::vec3) * vbo_grass_data.size(), &vbo_grass_data[0]);
   }*/

   //////////////////////////////////////////////////////////////////////////////////////

   _programs[0]->use();

   util::updateMVP(*_programs[0], glm::mat4(1.f), camera.getMatrix(), projection);

   // We bind all 5 textures - 3 of them are textures for layers, 1 texture is a "path" texture, and last one is
   // the places in heightmap where path should be and how intense should it be
   for (int i = 0; i < 6; ++i) {
      _programs[0]->activeTexture("gSampler[" + util::string(i) + "]", *_textures[i]);
   }
   
   glUniform3fv(_programs[0]->getUniformLocation("fScale"), 1, &_scale[0]);
   glUniform1f(_programs[0]->getUniformLocation("fMaxTextureU"), float(_width) * 0.1f);
   glUniform1f(_programs[0]->getUniformLocation("fMaxTextureV"), float(_height) * 0.1f);

   // Now we're ready to render - we are drawing set of triangle strips using one call, but we gotta enable primitive restart
   _vaos[0]->bind();
   glEnable(GL_PRIMITIVE_RESTART);
   glPrimitiveRestartIndex(_height * _width);

   int iNumIndices = (_height - 1) * _width * 2 + _height - 1;
   glDrawElements(GL_TRIANGLE_STRIP, iNumIndices, GL_UNSIGNED_INT, 0);
   _vaos[0]->unbind();

   glDisable(GL_PRIMITIVE_RESTART);

   //////////////////////////////////////////////////////////////////////////////////////

   if (!grass) return;

   _programs[1]->use();

   util::updateMVP(*_programs[1], glm::mat4(1.f), camera.getMatrix(), projection);

   // grassPack.dds
   _programs[1]->activeTexture("gSampler", *_textures[6]);

   glUniform3fv(_programs[1]->getUniformLocation("fScale"), 1, &_scale[0]);
   glUniform1f(_programs[1]->getUniformLocation("fTimePassed"), (float)glfwGetTime());

   glDisable(GL_CULL_FACE);

   _vaos[1]->bind();
   glDrawArrays(GL_POINTS, 0, _numGrassTrinagles);

   glEnable(GL_CULL_FACE);

   _vaos[1]->unbind();
}
