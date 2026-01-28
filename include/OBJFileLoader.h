#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include "glm.hpp"
#include "RawModel.h"
#include "Loader.h"
#include "Vertex.h"

class OBJFileLoader{
public:
    static RawModel loadOBJ(const std::string& objFileName, Loader& loader);

private:

    static Vertex* processVertex(const std::vector<std::string>& vertexData, 
                              std::vector<Vertex*>& vertices,
                              std::vector<int>& indices);
    
    static void calculateTangents(Vertex* v0, Vertex* v1, Vertex* v2,
                                const std::vector<glm::vec2>& textures);
    
    static float convertDataToArrays(const std::vector<Vertex*>& vertices,
                                   const std::vector<glm::vec2>& textures,
                                   const std::vector<glm::vec3>& normals,
                                   std::vector<float>& verticesArray,
                                   std::vector<float>& texturesArray,
                                   std::vector<float>& normalsArray,
                                   std::vector<float>& tangentsArray);

    static Vertex* dealWithAlreadyProcessedVertex(Vertex* previousVertex, 
                                               int newTextureIndex,
                                               int newNormalIndex,
                                               std::vector<int>& indices,
                                               std::vector<Vertex*>& vertices);
    
    static void removeUnusedVertices(std::vector<Vertex*>& vertices);
    
};


