#include "stdafx.h"
#include "gfx/Scene.h"

namespace gfx {

    Scene::Scene(util::string filename) : _filename(filename) {}

    Scene::~Scene(void) {        
        for (unsigned int n = 0; n < _meshes.size(); ++n) {
            delete _meshes[n];
        }
    }

    bool Scene::load(void) {
        unsigned int pFlags;  

        pFlags = aiProcess_CalcTangentSpace		|  \
            aiProcess_GenNormals			    |  \
            aiProcess_JoinIdenticalVertices		|  \
            aiProcess_ImproveCacheLocality		|  \
            aiProcess_LimitBoneWeights			|  \
            aiProcess_RemoveRedundantMaterials  |  \
            aiProcess_SplitLargeMeshes			|  \
            aiProcess_Triangulate				|  \
            aiProcess_GenUVCoords               |  \
            aiProcess_SortByPType               |  \
            aiProcess_FindDegenerates           |  \
            aiProcess_FindInvalidData           |  \
            aiProcess_PreTransformVertices;

        scene = importer.ReadFile(_filename, pFlags);
        if (scene == NULL) {
            std::cout << "Mesh::load () - Enable to load " << _filename << std::endl;
            std::cout << "Assimp Error: " << importer.GetErrorString() << std::endl;
            return false;
        } else {
            std::cout << "Loading: " << _filename << std::endl;
        }

        // figure out the path without the filename
        util::string path = _filename;
        path = path.substr(0, path.find_last_of ("/") + 1);

        aiVector3D vMin (1e10f,1e10f,1e10f);
        aiVector3D vMax (-1e10f,-1e10f,-1e10f);

        // For each mesh
        for (unsigned int n = 0; n < scene->mNumMeshes; ++n) {
            const aiMesh* mesh = scene->mMeshes[n];

            // create array with faces
            // have to convert from Assimp format to array
            unsigned int *faceArray;
            faceArray = (unsigned int *)malloc(sizeof(unsigned int) * mesh->mNumFaces * 3);
            unsigned int faceIndex = 0;

            for (unsigned int t = 0; t < mesh->mNumFaces; ++t) {
                const aiFace* face = &mesh->mFaces[t];

                for (unsigned int i = 0; i < face->mNumIndices; ++i) {
                    // bounding box
                    vMin.x = std::min(vMin.x, mesh->mVertices[face->mIndices[i]].x);
                    vMin.y = std::min(vMin.y, mesh->mVertices[face->mIndices[i]].y);
                    vMin.z = std::min(vMin.z, mesh->mVertices[face->mIndices[i]].z);

                    vMax.x = std::max(vMax.x, mesh->mVertices[face->mIndices[i]].x);
                    vMax.y = std::max(vMax.x, mesh->mVertices[face->mIndices[i]].y);
                    vMax.z = std::max(vMax.x, mesh->mVertices[face->mIndices[i]].z);
                }


                memcpy(&faceArray[faceIndex], face->mIndices,3 * sizeof(unsigned int));
                faceIndex += 3;
            }
            
            _meshes.emplace_back(new Mesh());            
            struct Mesh *aMesh = _meshes.back(); 

            // buffer for faces
            aMesh->vbo_indices = new glsl::VertexBufferObject(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
            aMesh->vbo_indices->bind();
            aMesh->vbo_indices->bufferData(sizeof(unsigned int) * mesh->mNumFaces * 3, faceArray);

            aMesh->numFaces = scene->mMeshes[n]->mNumFaces;

            // generate Vertex Array for mesh
            aMesh->vao = new glsl::VertexArrayObject();
            aMesh->vao->bind();
            {
                // buffer for vertex positions
                if (mesh->HasPositions()) {
                    std::cout << "[HasPositions]" << std::endl;

                    aMesh->vbo_positions = new glsl::VertexBufferObject(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
                    aMesh->vbo_positions->bind();
                    aMesh->vbo_positions->bufferData(sizeof(float) * 3 * mesh->mNumVertices, mesh->mVertices);

                    GLuint vertexLocation = 0;
                    glEnableVertexAttribArray(vertexLocation);
                    glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
                }

                // buffer for vertex normals
                if (mesh->HasNormals()) {
                    std::cout << "[HasNormals]" << std::endl;

                    aMesh->vbo_normals = new glsl::VertexBufferObject(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
                    aMesh->vbo_normals->bind();
                    aMesh->vbo_normals->bufferData(sizeof(float) * 3 * mesh->mNumVertices, mesh->mNormals);

                    GLuint normalLocation = 1;
                    glEnableVertexAttribArray(normalLocation);
                    glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
                }

                // buffer for vertex texture coordinates
                if (mesh->HasTextureCoords(0)) {
                    std::cout << "[HasTextureCoords]" << std::endl;

                    float *texCoords = (float *)malloc(sizeof(float) * 2 * mesh->mNumVertices);
                    for (unsigned int k = 0; k < mesh->mNumVertices; ++k) {

                        texCoords[k * 2]   = mesh->mTextureCoords[0][k].x;
                        texCoords[k * 2 + 1] = mesh->mTextureCoords[0][k].y; 

                    }
                    aMesh->vbo_texCoords = new glsl::VertexBufferObject(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
                    aMesh->vbo_texCoords->bind();
                    aMesh->vbo_texCoords->bufferData(sizeof(float) * 2 * mesh->mNumVertices, texCoords);

                    GLuint texcoordLocation = 2;
                    glEnableVertexAttribArray(texcoordLocation);
                    glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, 0, 0, 0);
                }
            }
            aMesh->vao->unbind();
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            std::cout << "[Material " << mesh->mMaterialIndex << ", Mesh " << n << "]" << std::endl;

            // create material uniform buffer
            aiMaterial *mtl = scene->mMaterials[mesh->mMaterialIndex];

            aiString texPath;	//contains filename of texture
            if(AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, 0, &texPath)){
                // generate texture
                aMesh->tex = new glsl::Texture2D(glsl::Texture2D::TEXTURE_2D);
                aMesh->tex->load(path + texPath.data);
                aMesh->aMat.texCount = 1;
            } else {
                aMesh->aMat.texCount = 0;
            }

            glm::vec4 color;

            color = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);		
            aiColor4D diffuse;
            if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
                memcpy(&color, &diffuse, sizeof(glm::vec4));
            memcpy(&aMesh->aMat.diffuse, &color, sizeof(glm::vec4));
            std::cout << "Diffuse [" << glm::to_string(color) << "]" << std::endl;

            color = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);	
            aiColor4D ambient;
            if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
                memcpy(&color, &ambient, sizeof(glm::vec4));
            memcpy(&aMesh->aMat.ambient, &color, sizeof(glm::vec4));
            std::cout << "Ambient [" << glm::to_string(color) << "]" << std::endl;

            color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);	
            aiColor4D specular;
            if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
                memcpy(&color, &specular, sizeof(glm::vec4));
            memcpy(&aMesh->aMat.specular, &color, sizeof(glm::vec4));
            std::cout << "Specular [" << glm::to_string(color) << "]" << std::endl;

            color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0);	
            aiColor4D emissive;
            if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emissive))
                memcpy(&color, &emissive, sizeof(glm::vec4));
            memcpy(&aMesh->aMat.emissive, &color, sizeof(glm::vec4));
            std::cout << "Emissive [" << glm::to_string(color) << "]" << std::endl;

            float shininess = 0.0;
            aiGetMaterialFloat(mtl, AI_MATKEY_SHININESS, &shininess);
            aMesh->aMat.shininess = shininess / 4.f;
            std::cout << "Shininess [" << aMesh->aMat.shininess << "]" << std::endl;

            float opacity = 1.0;
            aiGetMaterialFloat(mtl, AI_MATKEY_OPACITY, &opacity);
            aMesh->aMat.opacity = opacity;
            std::cout << "Opacity [" << aMesh->aMat.opacity << "]\n" << std::endl;
        }

        _scale = 20.0f / std::max(vMax.x - vMin.x, std::max(vMax.y - vMin.y, vMax.z - vMin.z));
        return true;
    }

    void Scene::loadTextures(void) {
    }

    void Scene::render(glsl::ShaderProgram& program) {
        GLint location = program.getUniformLocation("texUnit");
        if (location != -1) glUniform1i(location, 0);

        recursiveRender(program, scene->mRootNode);   
    }


    void Scene::recursiveRender(glsl::ShaderProgram& program, const aiNode *node) {
        // draw all meshes assigned to this node
        for (unsigned int n = 0; n < node->mNumMeshes; ++n){
            glUniform4fv(program.getUniformLocation("material.diffuse"), 1, &_meshes[node->mMeshes[n]]->aMat.diffuse[0]);
            glUniform4fv(program.getUniformLocation("material.ambient"), 1, &_meshes[node->mMeshes[n]]->aMat.ambient[0]);
            glUniform4fv(program.getUniformLocation("material.specular"), 1, &_meshes[node->mMeshes[n]]->aMat.specular[0]);
            glUniform4fv(program.getUniformLocation("material.emission"), 1, &_meshes[node->mMeshes[n]]->aMat.emissive[0]);
            glUniform1f(program.getUniformLocation("material.shininess"), _meshes[node->mMeshes[n]]->aMat.shininess);
            glUniform1f(program.getUniformLocation("material.opacity"), _meshes[node->mMeshes[n]]->aMat.opacity);
            glUniform1i(program.getUniformLocation("material.texCount"), _meshes[node->mMeshes[n]]->aMat.texCount);

            // bind texture
            if (_meshes[node->mMeshes[n]]->tex) {
                _meshes[node->mMeshes[n]]->tex->bind();
            }      

            // bind VAO
            _meshes[node->mMeshes[n]]->vao->bind();
            {
                // draw
                _meshes[node->mMeshes[n]]->vbo_indices->bind();
                glDrawElements(GL_TRIANGLES, _meshes[node->mMeshes[n]]->numFaces * 3, GL_UNSIGNED_INT, 0);
            }      
            _meshes[node->mMeshes[n]]->vao->unbind();
        }

        // draw all children
        for (unsigned int n = 0; n < node->mNumChildren; ++n){
            recursiveRender(program, node->mChildren[n]);
        }
    }

} // namespace gfx