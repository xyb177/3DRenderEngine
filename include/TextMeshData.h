#pragma once
#include <vector>

class TextMeshData {
public:
    /**
     * Stores vertex data for text rendering quads
     * 
     * @param vertexPositions - array of vertex positions
     * @param textureCoords - array of texture coordinates
     */
    TextMeshData(std::vector<float>&& vertexPositions, 
                std::vector<float>&& textureCoords);

    // Getters
    const std::vector<float>& getVertexPositions() const noexcept { return vertexPositions; }
    const std::vector<float>& getTextureCoords() const noexcept { return textureCoords; }
    const int getVertexCount() const noexcept { return vertexPositions.size() / 2; }

private:
    std::vector<float> vertexPositions;
    std::vector<float> textureCoords;
};
