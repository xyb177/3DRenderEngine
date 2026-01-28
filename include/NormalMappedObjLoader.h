#pragma once
#include <string>
#include <vector>
#include <memory>
#include "ModelDataNM.h"
#include "VertexNM.h"
#include "RawModel.h"
#include "Loader.h"
class NormalMappedObjLoader {

public:
    static RawModel loadOBJ(const std::string& objFileName, Loader& loader);    
private:
    
    static VertexNM* processVertex(const std::vector<std::string>& vertex, 
                                 std::vector<VertexNM*>& vertices,
                                 std::vector<int>& indices);
    
    static void calculateTangents(VertexNM* v0, VertexNM* v1, VertexNM* v2,
                                const std::vector<glm::vec2>& textures);
    
    static float convertDataToArrays(const std::vector<VertexNM*>& vertices,
                                   const std::vector<glm::vec2>& textures,
                                   const std::vector<glm::vec3>& normals,
                                   std::vector<float>& verticesArray,
                                   std::vector<float>& texturesArray,
                                   std::vector<float>& normalsArray,
                                   std::vector<float>& tangentsArray);
    
    static VertexNM* dealWithAlreadyProcessedVertex(VertexNM* previousVertex,
                                                  int newTextureIndex,
                                                  int newNormalIndex,
                                                  std::vector<int>& indices,
                                                  std::vector<VertexNM*>& vertices);
    
    static void removeUnusedVertices(std::vector<VertexNM*>& vertices);

};