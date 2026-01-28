#include "WaterRenderer.h"
#include "DisplayManager.h"
std::string WaterRenderer::DUDV_MAP = "res/waterDUDV.png";
std::string WaterRenderer::NORMAL_MAP = "res/normal.png";
WaterRenderer::WaterRenderer(Loader &loader, WaterShader &shader, const glm::mat4 &projectionMatrix, WaterFrameBuffers &fbos)
 : shader(shader),fbos(fbos) {
    dudvTexture = loader.loadTexture(DUDV_MAP);
    normalMap = loader.loadTexture(NORMAL_MAP);
    shader.start();
    shader.connectTextureUnits();
    shader.loadProjectionMatrix(projectionMatrix);
    shader.stop();
    setUpVAO(loader);
}

void WaterRenderer::render(const std::vector<WaterTile> &water, const Camera &camera, const Light& sun)
{
    prepareRender(camera,sun);
    
    for (const auto& tile : water) {
        glm::mat4 modelMatrix = Maths::createTransformationMatrix(
            glm::vec3(tile.getX(), tile.getHeight(), tile.getZ()),
            0, 0, 0,
            WaterTile::TILE_SIZE);
        
        shader.loadModelMatrix(modelMatrix);
        glDrawArrays(GL_TRIANGLES, 0, quad.getVertexCount());
    }
    
    unbind();
}

void WaterRenderer::prepareRender(const Camera& camera, const Light& sun) {
    shader.start();
    shader.loadViewMatrix(camera);
    moveFactor += WAVE_SPEED * DisplayManager::getInstance().getFrameTimeSeconds();
    std::fmod(moveFactor,1);
    shader.loadMoveFactor(moveFactor);
    shader.loadLight(sun);
    glBindVertexArray(quad.getVaoID());
    glEnableVertexAttribArray(0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fbos.getReflectionTexture());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, fbos.getRefractionTexture());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, dudvTexture);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, normalMap);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, fbos.getRefractionDepthTexture());

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

}

void WaterRenderer::unbind() {
    glDisable(GL_BLEND);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
    shader.stop();
}

void WaterRenderer::setUpVAO(Loader& loader) {
    // Just x and z vertex positions here, y is set to 0 in vertex shader
    std::vector<float>vertices = { -1, -1, -1, 1, 1, -1, 1, -1, -1, 1, 1, 1 };
    quad = loader.loadToVAO(vertices,2);
}