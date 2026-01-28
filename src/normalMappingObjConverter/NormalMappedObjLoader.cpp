#include "NormalMappedObjLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "glm.hpp"
#include <algorithm>

RawModel NormalMappedObjLoader::loadOBJ(const std::string& objFileName, Loader& loader) {
    std::ifstream file(objFileName);
    if(!file.is_open()) {
        std::string errMsg = "Failed to open OBJ file: " + objFileName;
        std::cerr << "ERROR::OBJLOADER::" << errMsg << std::endl;
        throw std::runtime_error(errMsg);
    }

    std::vector<VertexNM*> vertices;
    std::vector<glm::vec2> textures;
    std::vector<glm::vec3> normals;
    std::vector<int> indices;
    
    std::string line;
    while(std::getline(file, line)) {
        if(line.empty()) continue;
        
        if(line[0] == 'v') {
            if(line[1] == ' ') { // vertex
                std::istringstream iss(line.substr(2));
                glm::vec3 vertex;
                iss >> vertex.x >> vertex.y >> vertex.z;
                vertices.push_back(new VertexNM(vertices.size(), vertex));
            }
            else if(line[1] == 't') { // texture
                std::istringstream iss(line.substr(3));
                glm::vec2 texture;
                iss >> texture.x >> texture.y;
                textures.push_back(texture);
            }
            else if(line[1] == 'n') { // normal
                std::istringstream iss(line.substr(3));
                glm::vec3 normal;
                iss >> normal.x >> normal.y >> normal.z;
                normals.push_back(normal);
            }
        }
        else if(line[0] == 'f') {
            break;
        }
    }
    
    // Process faces
    do {
        if(line.empty() || line[0] != 'f') continue;
        
        std::istringstream iss(line.substr(2));
        std::vector<std::string> vertexStrings(3);
        iss >> vertexStrings[0] >> vertexStrings[1] >> vertexStrings[2];
       
        std::vector<std::vector<std::string>> verticesData(3);
        for(int i = 0; i < 3; ++i) {
            size_t pos = 0;
            while((pos = vertexStrings[i].find('/')) != std::string::npos) {
                verticesData[i].push_back(vertexStrings[i].substr(0, pos));
                vertexStrings[i].erase(0, pos + 1);
            }
            verticesData[i].push_back(vertexStrings[i]);
        }
       
        VertexNM* v0 = processVertex(verticesData[0], vertices, indices);
        VertexNM* v1 = processVertex(verticesData[1], vertices, indices);
        VertexNM* v2 = processVertex(verticesData[2], vertices, indices);
       
        calculateTangents(v0, v1, v2, textures);
       
    } while(std::getline(file, line));
    
    file.close();
    
    removeUnusedVertices(vertices);
    
    std::vector<float> verticesArray(vertices.size() * 3);
    std::vector<float> texturesArray(vertices.size() * 2);
    std::vector<float> normalsArray(vertices.size() * 3);
    std::vector<float> tangentsArray(vertices.size() * 3);
    
    float furthest = convertDataToArrays(vertices, textures, normals, 
                                       verticesArray, texturesArray, 
                                       normalsArray, tangentsArray);
    
    return loader.loadToVAO(verticesArray,texturesArray, normalsArray, tangentsArray, indices);
}

void NormalMappedObjLoader::calculateTangents(
    VertexNM* v0, VertexNM* v1, VertexNM* v2,
    const std::vector<glm::vec2>& textures) {
    
    glm::vec3 delatPos1 = v1->getPosition() - v0->getPosition();
    glm::vec3 delatPos2 = v2->getPosition() - v0->getPosition();

    glm::vec2 uv0 = textures[v0->getTextureIndex()];
    glm::vec2 uv1 = textures[v1->getTextureIndex()];
    glm::vec2 uv2 = textures[v2->getTextureIndex()];
   
    glm::vec2 deltaUv1 = uv1 - uv0;
    glm::vec2 deltaUv2 = uv2 - uv0;

    float r = 1.0f / (deltaUv1.x * deltaUv2.y - deltaUv1.y * deltaUv2.x);
    delatPos1 *= deltaUv2.y;
    delatPos2 *= deltaUv1.y;
    
    glm::vec3 tangent = delatPos1 - delatPos2;
    tangent *= r;
   
    v0->addTangent(tangent);
    v1->addTangent(tangent);
    v2->addTangent(tangent);
   
}

VertexNM* NormalMappedObjLoader::processVertex(
    const std::vector<std::string>& vertex,
    std::vector<VertexNM*>& vertices,
    std::vector<int>& indices) {
    
    int index = std::stoi(vertex[0]) - 1;
    VertexNM* currentVertex = vertices[index];
    int textureIndex = std::stoi(vertex[1]) - 1;
    int normalIndex = std::stoi(vertex[2]) - 1;

    if (!currentVertex->isSet()) {
        currentVertex->setTextureIndex(textureIndex);
        currentVertex->setNormalIndex(normalIndex);
        indices.push_back(index);
        return currentVertex;
    } else {
        return dealWithAlreadyProcessedVertex(
            currentVertex, textureIndex, normalIndex, indices, vertices);
    }
}


float NormalMappedObjLoader::convertDataToArrays(
    const std::vector<VertexNM*>& vertices,
    const std::vector<glm::vec2>& textures,
    const std::vector<glm::vec3>& normals,
    std::vector<float>& verticesArray,
    std::vector<float>& texturesArray,
    std::vector<float>& normalsArray,
    std::vector<float>& tangentsArray) {
    
    float furthestPoint = 0;
    for (size_t i = 0; i < vertices.size(); ++i) {
        const auto& currentVertex = vertices[i];
        if (currentVertex->getLength() > furthestPoint) {
            furthestPoint = currentVertex->getLength();
        }

        const glm::vec3& position = currentVertex->getPosition();
        const glm::vec2& textureCoord = textures[currentVertex->getTextureIndex()];
        const glm::vec3& normalVector = normals[currentVertex->getNormalIndex()];
        const glm::vec3& tangent = currentVertex->getAverageTangent();

        verticesArray[i * 3] = position.x;
        verticesArray[i * 3 + 1] = position.y;
        verticesArray[i * 3 + 2] = position.z;
        
        texturesArray[i * 2] = textureCoord.x;
        texturesArray[i * 2 + 1] = 1.0f - textureCoord.y;
        
        normalsArray[i * 3] = normalVector.x;
        normalsArray[i * 3 + 1] = normalVector.y;
        normalsArray[i * 3 + 2] = normalVector.z;
        
        tangentsArray[i * 3] = tangent.x;
        tangentsArray[i * 3 + 1] = tangent.y;
        tangentsArray[i * 3 + 2] = tangent.z;
    }
    return furthestPoint;
}

VertexNM* NormalMappedObjLoader::dealWithAlreadyProcessedVertex(
    VertexNM* previousVertex,
    int newTextureIndex,
    int newNormalIndex,
    std::vector<int>& indices,
    std::vector<VertexNM*>& vertices) {
    
    if (previousVertex->hasSameTextureAndNormal(newTextureIndex, newNormalIndex)) {
        indices.push_back(previousVertex->getIndex());
        return previousVertex;
    }

    VertexNM* anotherVertex = previousVertex->getDuplicateVertex();
    if (anotherVertex != nullptr) {
        return dealWithAlreadyProcessedVertex(
            anotherVertex, newTextureIndex, newNormalIndex, indices, vertices);
    }

    auto duplicateVertex = new VertexNM(
        vertices.size(), previousVertex->getPosition());
    
    duplicateVertex->setTextureIndex(newTextureIndex);
    duplicateVertex->setNormalIndex(newNormalIndex);
    previousVertex->setDuplicateVertex(duplicateVertex);
    vertices.push_back(duplicateVertex);
    indices.push_back(duplicateVertex->getIndex());
    
    return duplicateVertex;
}

void NormalMappedObjLoader::removeUnusedVertices(
    std::vector<VertexNM*>& vertices) {
    
    for (auto& vertex : vertices) {
        vertex->averageTangents();
        if (!vertex->isSet()) {
            vertex->setTextureIndex(0);
            vertex->setNormalIndex(0);
        }
    }
}