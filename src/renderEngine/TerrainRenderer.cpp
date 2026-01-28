#include "TerrainRenderer.h"



TerrainRenderer::TerrainRenderer(TerrainShader &shader,  const glm::mat4 &projectionMatrix)
:shader(shader)
{
    shader.start();
    shader.loadProjectionMatrix(projectionMatrix);
    shader.stop();
}

void TerrainRenderer::render(const std::vector<Terrain>& terrains) {
    for (const auto& terrain : terrains) {
        prepareTerrain(terrain);
        loadModelMatrix(terrain);
        glDrawElements(GL_TRIANGLES, terrain.getModel().getVertexCount(),
                      GL_UNSIGNED_INT, nullptr);
        unbindTexturedModel();
    }
}

void TerrainRenderer::render(const std::vector<Terrain>& terrains, glm::mat4 toShadowSpace) {
    shader.loadToShadowSpaceMatrix(toShadowSpace);
    for (const auto& terrain : terrains) {
        prepareTerrain(terrain);
        loadModelMatrix(terrain);
        glDrawElements(GL_TRIANGLES, terrain.getModel().getVertexCount(),
                      GL_UNSIGNED_INT, nullptr);
        unbindTexturedModel();
    }
}

void TerrainRenderer::prepareTerrain(const Terrain& terrain) {
    const auto& rawModel = terrain.getModel();
    glBindVertexArray(rawModel.getVaoID());
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    
    bindTextures(terrain);
    shader.connectTextureUnits();
    shader.loadShineVariables(1, 0);
    
}

void TerrainRenderer::unbindTexturedModel() {
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindVertexArray(0);
}

void TerrainRenderer::loadModelMatrix(const Terrain& terrain) {
    glm::mat4 transformationMatrix = Maths::createTransformationMatrix(
        glm::vec3(terrain.getX(), 0.0f, terrain.getZ()), 
        0.0f, 0.0f, 0.0f, 1.0f);
    shader.loadTransformationMatrix(transformationMatrix);
}

void TerrainRenderer::bindTextures(const Terrain &terrain)
{
    TerrainTexturePack texturePack = terrain.getTexturePack();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texturePack.getBackGroundTexture().getTextureID());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texturePack.getrTexture().getTextureID());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texturePack.getgTexture().getTextureID());
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, texturePack.getbTexture().getTextureID());
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, terrain.getBlendMap().getTextureID());
}
