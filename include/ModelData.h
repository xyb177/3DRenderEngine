#pragma once
#include <vector>

class ModelData {
private:
    std::vector<float> vertices;
    std::vector<float> textureCoords;
    std::vector<float> normals;
    std::vector<int> indices;
    float furthestPoint;
    
public:
    ModelData(const std::vector<float>& vertices, 
              const std::vector<float>& textureCoords,
              const std::vector<float>& normals,
              const std::vector<int>& indices,
              float furthestPoint);
    
    const std::vector<float>& getVertices() const;
    const std::vector<float>& getTextureCoords() const;
    const std::vector<float>& getNormals() const;
    const std::vector<int>& getIndices() const;
    float getFurthestPoint() const;
};
