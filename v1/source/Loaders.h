#ifndef __LOADERS_H__
#define __LOADERS_H__

GLuint loadBMP(const char * imagepath);
bool loadOBJ(const char * path, 
   std::vector<glm::vec3> & out_vertices, 
   std::vector<glm::vec2> & out_uvs,
   std::vector<glm::vec3> & out_normals
);

#endif // __LOADERS_H__