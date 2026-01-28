#include "NormalMappingRenderer.h"
#include "MasterRenderer.h"
#include "Maths.h"
#include "glad/glad.h"

NormalMappingRenderer::NormalMappingRenderer(const glm::mat4& projectionMatrix) {
    shader.start();
    shader.loadProjectionMatrix(projectionMatrix);
    shader.connectTextureUnits();
    shader.stop();
}

void NormalMappingRenderer::render(const std::unordered_map<TexturedModel, std::vector<Entity>>& entities,
                                 const glm::vec4& clipPlane, const std::vector<Light>& lights,
                                 const Camera& camera) {
    shader.start();
    prepare(clipPlane, lights, camera);
    
    for (auto& entry : entities)
    {   
        const TexturedModel& model = entry.first;
        const std::vector<Entity>& batch = entry.second;
        prepareTexturedModel(model);
         
        for (const Entity& entity : batch) {
            prepareInstance(entity); 
            glDrawElements(GL_TRIANGLES, model.getRawModel().getVertexCount(), GL_UNSIGNED_INT, nullptr);
            
        }
        unbindTexturedModel();
    }
    shader.stop();
}

void NormalMappingRenderer::cleanUp() {
    shader.cleanUp();
}

void NormalMappingRenderer::prepareTexturedModel(const TexturedModel& model) {
    const RawModel& rawModel = model.getRawModel();
    glBindVertexArray(rawModel.getVaoID());
   
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    
    const ModelTexture& texture = model.getTexture();
    shader.loadNumberOfRows(texture.getNumberOfRows());
    
    if (texture.isHasTransparency()) {
        glDisable(GL_CULL_FACE);
    }
    
    shader.loadShineVariables(texture.getShineDamper(), texture.getReflectivity());
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, model.getTexture().getID());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, model.getTexture().getNormalMap());

    shader.loadUseSpecularMap(texture.isHasSpecularMap());
    if(texture.isHasSpecularMap()){
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture.getSpecularMap());
    }
}

void NormalMappingRenderer::unbindTexturedModel() {
    MasterRenderer::enableCulling();
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glBindVertexArray(0);
}

void NormalMappingRenderer::prepareInstance(const Entity& entity) {
    glm::mat4 transformationMatrix = Maths::createTransformationMatrix(
        entity.getPosition(), entity.getRotX(), entity.getRotY(), 
        entity.getRotZ(), entity.getScale());
    shader.loadTransformationMatrix(transformationMatrix);
    shader.loadOffset(entity.getTextureXOffset(), entity.getTextureYOffset());
}

void NormalMappingRenderer::prepare(const glm::vec4& clipPlane, 
                                  const std::vector<Light>& lights, 
                                  const Camera& camera) {
    shader.loadClipPlane(clipPlane);
    shader.loadSkyColour(MasterRenderer::RED, MasterRenderer::GREEN, MasterRenderer::BLUE);
    glm::mat4 viewMatrix = Maths::createViewMatrix(camera);
    
    shader.loadLights(lights, viewMatrix);
    shader.loadViewMatrix(viewMatrix);
}
