#pragma once
#include <vector>


class ModelDataNM {

public:
    ModelDataNM(const std::vector<float>& vertices, 
               const std::vector<float>& textureCoords,
               const std::vector<float>& normals,
               const std::vector<float>& tangents,
               const std::vector<unsigned int>& indices,
               float furthestPoint);

    const std::vector<float>& getVertices() const;
    const std::vector<float>& getTextureCoords() const;
    const std::vector<float>& getTangents() const;
    const std::vector<float>& getNormals() const;
    const std::vector<unsigned int>& getIndices() const;
    float getFurthestPoint() const;    
    
private:
    std::vector<float> vertices;
    std::vector<float> textureCoords;
    std::vector<float> normals;
    std::vector<float> tangents;
    std::vector<unsigned int> indices;
    float furthestPoint;


};