#pragma once
#include <vector>
#include <memory>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "TerrainShader.h"
#include "Terrain.h"
#include "RawModel.h"
#include "ModelTexture.h"
#include "Maths.h"
#include "TerrainTexturePack.h"

class TerrainRenderer {
public:
    TerrainRenderer(TerrainShader& shader, const glm::mat4& projectionMatrix);
    void render(const std::vector<Terrain>& terrains);
    void render(const std::vector<Terrain>& terrains, glm::mat4 toShadowSpace);
private:
    void prepareTerrain(const Terrain& terrain);
    void unbindTexturedModel();
    void loadModelMatrix(const Terrain& terrain);
    void bindTextures(const Terrain& terrain);

    TerrainShader& shader;
};
