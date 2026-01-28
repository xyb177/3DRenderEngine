#include "ModelDataNM.h"

ModelDataNM::ModelDataNM(const std::vector<float>& vertices,
                       const std::vector<float>& textureCoords,
                       const std::vector<float>& normals,
                       const std::vector<float>& tangents,
                       const std::vector<unsigned int>& indices,
                       float furthestPoint)
    : vertices(vertices), textureCoords(textureCoords), 
      normals(normals), tangents(tangents), 
      indices(indices), furthestPoint(furthestPoint) {}

const std::vector<float>& ModelDataNM::getVertices() const { return vertices; }
const std::vector<float>& ModelDataNM::getTextureCoords() const { return textureCoords; }
const std::vector<float>& ModelDataNM::getTangents() const { return tangents; }
const std::vector<float>& ModelDataNM::getNormals() const { return normals; }
const std::vector<unsigned int>& ModelDataNM::getIndices() const { return indices; }
float ModelDataNM::getFurthestPoint() const { return furthestPoint; }
