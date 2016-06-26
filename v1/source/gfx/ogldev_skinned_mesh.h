/*

	Copyright 2011 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OGLDEV_SKINNED_MESH_H
#define	OGLDEV_SKINNED_MESH_H

#include "gfx/ogldev_util.h"
#include "gfx/ogldev_math_3d.h"

#include "glsl/ShaderProgram.h"
#include "glsl/TextureObject.h"

using namespace std;

class SkinnedMesh
{
public:
   static const uint MAX_BONES = 100;

    SkinnedMesh();

    ~SkinnedMesh();

    bool LoadMesh(const string& Filename);

    void render(glsl::ShaderProgram& program);
	
    uint NumBones() const
    {
        return m_NumBones;
    }

    glm::mat4 translate_to_origin_and_scale(glm::mat4 model) {
       glm::mat4 transformation(model);

       float x = 0.f, y = 0.f, z = 0.f;

       if (vMin.x > 0.f) {
          x = - (vMin.x + vMax.x) / 2.f;
       } else {
          x = (-vMin.x - vMax.x) / 2.f;
       }

       if (vMin.y > 0.f) {
          y = - (vMin.y + vMax.y) / 2.f;
       } else {
          y = (-vMin.y - vMax.y) / 2.f;
       }

       if (vMin.z > 0.f) {
          z = - (vMin.z + vMax.z) / 2.f;
       } else {
          z = (-vMin.z - vMax.z) / 2.f;
       }

       float scale = vMax.x - vMin.x;
       scale = vMax.y - vMin.y > scale ? vMax.y - vMin.y : scale;
       scale = vMax.z - vMin.z > scale ? vMax.z - vMin.z : scale;

       transformation = glm::scale(transformation, glm::vec3(1.f / scale));
       transformation = glm::translate(transformation, glm::vec3(x, y, z));

       return transformation;
    }
    
    void boneTransform(glsl::ShaderProgram& program, double TimeInSeconds);
    
private:
    #define NUM_BONES_PER_VEREX 4

    struct BoneInfo
    {
        Matrix4f BoneOffset;
        Matrix4f FinalTransformation;        

        BoneInfo()
        {
            BoneOffset.SetZero();
            FinalTransformation.SetZero();            
        }
    };
    
    struct VertexBoneData
    {        
        uint IDs[NUM_BONES_PER_VEREX];
        float Weights[NUM_BONES_PER_VEREX];

        VertexBoneData()
        {
            Reset();
        };
        
        void Reset()
        {
            ZERO_MEM(IDs);
            ZERO_MEM(Weights);        
        }
        
        void AddBoneData(uint BoneID, float Weight);
    };

    void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);    
    uint FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
    uint FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
    uint FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
    const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const string NodeName);
    void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const Matrix4f& ParentTransform);
    bool InitFromScene(const aiScene* pScene, const string& Filename);
    void InitMesh(uint MeshIndex,
                  const aiMesh* paiMesh,
                  vector<Vector3f>& Positions,
                  vector<Vector3f>& Normals,
                  vector<Vector2f>& TexCoords,
                  vector<VertexBoneData>& Bones,
                  vector<unsigned int>& Indices);
    void LoadBones(uint MeshIndex, const aiMesh* paiMesh, vector<VertexBoneData>& Bones);
    bool InitMaterials(const aiScene* pScene, const string& Filename);
    void Clear();

#define INVALID_MATERIAL 0xFFFFFFFF
  
enum VB_TYPES {
    INDEX_BUFFER,
    POS_VB,
    NORMAL_VB,
    TEXCOORD_VB,
    BONE_VB,
    NUM_VBs            
};

    GLuint m_VAO;
    GLuint m_Buffers[NUM_VBs];

    struct MeshEntry {
        MeshEntry()
        {
            NumIndices    = 0;
            BaseVertex    = 0;
            BaseIndex     = 0;
            MaterialIndex = INVALID_MATERIAL;
        }
        
        unsigned int NumIndices;
        unsigned int BaseVertex;
        unsigned int BaseIndex;
        unsigned int MaterialIndex;
    };

    struct Material {
       Material() {
          tex = NULL;
          texCount = 0;
       }

       ~Material() {
          delete tex;
          tex = NULL;
       }

       glm::vec4 diffuse;
       glm::vec4 ambient;
       glm::vec4 specular;
       glm::vec4 emissive;
       GLfloat shininess;
       GLfloat opacity;
       GLint texCount;

       glsl::Texture2D *tex;
    };
    
    vector<MeshEntry> m_Entries;
    vector<Material> m_Material;
     
    map<string,uint> m_BoneMapping; // maps a bone name to its index
    uint m_NumBones;
    vector<BoneInfo> m_BoneInfo;
    Matrix4f m_GlobalInverseTransform;
    
    const aiScene* m_pScene;
    Assimp::Importer m_Importer;

    aiVector3D vMin, vMax;
};


#endif	/* OGLDEV_SKINNED_MESH_H */

