#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include "RawModel.h"
#include "Loader.h"
#include "ModelData.h"
#include "Vertex.h"
#include "glm.hpp"

class OBJLoader {
public:
    static ModelData loadObjModel(const std::string& fileName);
    
private:
    static void processVertex(const std::vector<std::string>& vertexData,
                            std::vector<Vertex*>& vertices,
                            std::vector<int>& indices,
                            const std::vector<glm::vec2>& textures,
                            const std::vector<glm::vec3>& normals);

    static void dealWithAlreadyProcessedVertex(Vertex* previousVertex,
                                            int newTextureIndex,
                                            int newNormalIndex,
                                            std::vector<int>& indices,
                                            std::vector<Vertex*>& vertices);

    static float convertDataToArrays(std::vector<Vertex*>& vertices,
                                   const std::vector<glm::vec2>& textures,
                                   const std::vector<glm::vec3>& normals,
                                   std::vector<float>& verticesArray,
                                   std::vector<float>& texturesArray,
                                   std::vector<float>& normalsArray);

    static void removeUnusedVertices(std::vector<Vertex*>& vertices);
    static std::vector<std::string> split(const std::string& s, char delimiter);
};