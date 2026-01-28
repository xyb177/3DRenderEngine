#include "OBJLoader.h"


ModelData OBJLoader::loadObjModel(const std::string &fileName) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::string errMsg = "Failed to open OBJ file: " + fileName;
        std::cerr << "ERROR::OBJLOADER::" << errMsg << std::endl;
        throw std::runtime_error(errMsg);
    }

    std::vector<Vertex*> vertices;
    std::vector<glm::vec2> textures;
    std::vector<glm::vec3> normals;
    std::vector<int> indices;

    std::string line;
    try {
        // 第一阶段：读取顶点、纹理坐标和法线数据
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            
            std::istringstream iss(line);
            std::string type;
            iss >> type;
            
            if (type == "v") {
                glm::vec3 vertex;
                iss >> vertex.x >> vertex.y >> vertex.z;
                vertices.push_back(new Vertex(vertices.size(), vertex));
            } else if (type == "vt") {
                glm::vec2 texture;
                iss >> texture.x >> texture.y;
                textures.push_back(texture);
            } else if (type == "vn") {
                glm::vec3 normal;
                iss >> normal.x >> normal.y >> normal.z;
                normals.push_back(normal);
            } else if (type == "f") {
                break; // 开始处理面数据
            }
        }

        // 第二阶段：处理面数据
        do {
            if (line.empty() || line[0] != 'f') continue;
            
            std::istringstream iss(line.substr(2));
            std::string vertexData[3];
            iss >> vertexData[0] >> vertexData[1] >> vertexData[2];
            
            for (int i = 0; i < 3; i++) {
                std::vector<std::string> parts = split(vertexData[i], '/');
                processVertex(parts, vertices, indices, textures, normals);
            }
            
            // 处理多边形面（超过3个顶点的情况）
            std::string extraVertex;
            while (iss >> extraVertex) {
                std::vector<std::string> parts = split(vertexData[1], '/');
                processVertex(parts, vertices, indices, textures, normals);
                
                parts = split(extraVertex, '/');
                processVertex(parts, vertices, indices, textures, normals);
                
                vertexData[1] = extraVertex;
            }
        } while (std::getline(file, line));
        
        file.close();
    } catch (const std::exception& e) {
        file.close();
        for (Vertex* v : vertices) delete v;
        throw;
    }

    removeUnusedVertices(vertices);
    
    std::vector<float> verticesArray(vertices.size() * 3);
    std::vector<float> texturesArray(vertices.size() * 2);
    std::vector<float> normalsArray(vertices.size() * 3);
    
    float furthest = convertDataToArrays(vertices, textures, normals, 
                                       verticesArray, texturesArray, normalsArray);
    
    // 清理顶点数据
    for (Vertex* v : vertices) {
        delete v;
    }
    
    return ModelData(verticesArray, texturesArray, normalsArray, indices, furthest);
}



void OBJLoader::processVertex(const std::vector<std::string> &vertexData,
                              std::vector<Vertex *> &vertices,
                              std::vector<int> &indices,
                              const std::vector<glm::vec2> &textures,
                              const std::vector<glm::vec3> &normals)
{
    int index = std::stoi(vertexData[0]) - 1;
    Vertex* currentVertex = vertices[index];
    
    int textureIndex = vertexData.size() > 1 && !vertexData[1].empty() ? 
                      std::stoi(vertexData[1]) - 1 : -1;
    int normalIndex = vertexData.size() > 2 && !vertexData[2].empty() ? 
                     std::stoi(vertexData[2]) - 1 : -1;
    
    if (!currentVertex->isSet()) {
        currentVertex->setTextureIndex(textureIndex);
        currentVertex->setNormalIndex(normalIndex);
        indices.push_back(currentVertex->getIndex());
    } else {
        dealWithAlreadyProcessedVertex(currentVertex, textureIndex, normalIndex, 
                                      indices, vertices);
    }
}

void OBJLoader::dealWithAlreadyProcessedVertex(Vertex* previousVertex,
                                             int newTextureIndex,
                                             int newNormalIndex,
                                             std::vector<int>& indices,
                                             std::vector<Vertex*>& vertices) {
    if (previousVertex->hasSameTextureAndNormal(newTextureIndex, newNormalIndex)) {
        indices.push_back(previousVertex->getIndex());
    } else {
        Vertex* anotherVertex = previousVertex->getDuplicateVertex();
        if (anotherVertex != nullptr) {
            dealWithAlreadyProcessedVertex(anotherVertex, newTextureIndex, 
                                         newNormalIndex, indices, vertices);
        } else {
            Vertex* duplicateVertex = new Vertex(vertices.size(), 
                                               previousVertex->getPosition());
            duplicateVertex->setTextureIndex(newTextureIndex);
            duplicateVertex->setNormalIndex(newNormalIndex);
            previousVertex->setDuplicateVertex(duplicateVertex);
            vertices.push_back(duplicateVertex);
            indices.push_back(duplicateVertex->getIndex());
        }
    }
}

float OBJLoader::convertDataToArrays(std::vector<Vertex*>& vertices,
                                   const std::vector<glm::vec2>& textures,
                                   const std::vector<glm::vec3>& normals,
                                   std::vector<float>& verticesArray,
                                   std::vector<float>& texturesArray,
                                   std::vector<float>& normalsArray) {
    float furthestPoint = 0;
    for (size_t i = 0; i < vertices.size(); i++) {
        Vertex* currentVertex = vertices[i];
        if (currentVertex->getLength() > furthestPoint) {
            furthestPoint = currentVertex->getLength();
        }
        
        glm::vec3 position = currentVertex->getPosition();
        verticesArray[i * 3] = position.x;
        verticesArray[i * 3 + 1] = position.y;
        verticesArray[i * 3 + 2] = position.z;
        
        if (currentVertex->getTextureIndex() >= 0 && 
            currentVertex->getTextureIndex() < (int)textures.size()) {
            glm::vec2 textureCoord = textures[currentVertex->getTextureIndex()];
            texturesArray[i * 2] = textureCoord.x;
            texturesArray[i * 2 + 1] = 1.0f - textureCoord.y;
        }
        
        if (currentVertex->getNormalIndex() >= 0 && 
            currentVertex->getNormalIndex() < (int)normals.size()) {
            glm::vec3 normalVector = normals[currentVertex->getNormalIndex()];
            normalsArray[i * 3] = normalVector.x;
            normalsArray[i * 3 + 1] = normalVector.y;
            normalsArray[i * 3 + 2] = normalVector.z;
        }
    }
    return furthestPoint;
}

void OBJLoader::removeUnusedVertices(std::vector<Vertex*>& vertices) {
    for (Vertex* vertex : vertices) {
        if (!vertex->isSet()) {
            vertex->setTextureIndex(0);
            vertex->setNormalIndex(0);
        }
    }
}

std::vector<std::string> OBJLoader::split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}