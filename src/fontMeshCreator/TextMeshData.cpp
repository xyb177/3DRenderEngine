#include "TextMeshData.h"

TextMeshData::TextMeshData(std::vector<float>&& vertexPositions,
                          std::vector<float>&& textureCoords)
    : vertexPositions(std::move(vertexPositions)),
      textureCoords(std::move(textureCoords)) {}
