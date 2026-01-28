#pragma once 
#include "EntityRenderer.h"


EntityRenderer::EntityRenderer(StaticShader &shader, const glm::mat4 &projectionMatrix)
:shader(shader)
{   glEnable(GL_CULL_FACE);
    shader.start();
    shader.loadProjectionMatrix(projectionMatrix);
    shader.connectTextureUnits();
    shader.stop();
}


void EntityRenderer::render(const Entity& entity, StaticShader& shader)
{
    const TexturedModel& model = entity.getModel();
    const RawModel& rawModel = model.getRawModel();

    glBindVertexArray(rawModel.getVaoID());
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glm::mat4 transformationMatrix = Maths::createTransformationMatrix(
        entity.getPosition(),
        entity.getRotX(),
        entity.getRotY(),
        entity.getRotZ(),
        entity.getScale()
    );
    
    shader.loadTransformationMatrix(transformationMatrix);
    ModelTexture texture = model.getTexture();
    shader.loadShineVariables(texture.getShineDamper(),texture.getReflectivity());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, model.getTexture().getID());
    
    glDrawElements(GL_TRIANGLES, rawModel.getVertexCount(), GL_UNSIGNED_INT, nullptr);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindVertexArray(0);
}

void EntityRenderer::render(std::unordered_map<TexturedModel, std::vector<Entity>>& entities)
{
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
}

void EntityRenderer::prepareTexturedModel(const TexturedModel& model)
{
    const RawModel& rawModel = model.getRawModel();
    glBindVertexArray(rawModel.getVaoID());
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    const ModelTexture& texture = model.getTexture();
    shader.loadNumberOfRow(texture.getNumberOfRows());
    if (texture.isHasTransparency()){
        glDisable(GL_CULL_FACE);
    }
    
    shader.loadFakeLightingVariable(texture.isUseFakeLighting());
    shader.loadShineVariables(texture.getShineDamper(),texture.getReflectivity());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.getID());
    
    shader.loadUseSpecularMap(texture.isHasSpecularMap());
    if(texture.isHasSpecularMap())
    {   
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture.getSpecularMap());
    }

    shader.loadUseEnvironmentMap(texture.isHasEnvironmentMap(),texture.getReflectionStrength());
    if(texture.isHasEnvironmentMap()){
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture.getEnvironmentMap());
    }
}

void EntityRenderer::unbindTexturedModel()
{   glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindVertexArray(0);
}

void EntityRenderer::prepareInstance(const Entity& entity)
{
    glm::mat4 transformationMatrix = Maths::createTransformationMatrix(
        entity.getPosition(),
        entity.getRotX(),
        entity.getRotY(),
        entity.getRotZ(),
        entity.getScale()
    );
    shader.loadTransformationMatrix(transformationMatrix);
    shader.loadOffset(entity.getTextureXOffset(),entity.getTextureYOffset());
}


