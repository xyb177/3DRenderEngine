#include "ModelData.h"

ModelData::ModelData(const std::vector<float>& vertices,
                     const std::vector<float>& textureCoords,
                     const std::vector<float>& normals,
                     const std::vector<int>& indices,
                     float furthestPoint)
    : vertices(vertices), textureCoords(textureCoords), 
      normals(normals), indices(indices), furthestPoint(furthestPoint) {}

const std::vector<float>& ModelData::getVertices() const { 
    return vertices; 
}

const std::vector<float>& ModelData::getTextureCoords() const { 
    return textureCoords; 
}

const std::vector<float>& ModelData::getNormals() const { 
    return normals; 
}

const std::vector<int>& ModelData::getIndices() const { 
    return indices; 
}

float ModelData::getFurthestPoint() const { 
    return furthestPoint; 
}
