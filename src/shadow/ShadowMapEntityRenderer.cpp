#include "ShadowMapEntityRenderer.h"
#include "Maths.h"
#include <iostream>

ShadowMapEntityRenderer::ShadowMapEntityRenderer(ShadowShader &shader, const glm::mat4 &projectionViewMatrix)
                            :shader(shader),projectionViewMatrix(projectionViewMatrix)
{
}

void ShadowMapEntityRenderer::render(const std::unordered_map<TexturedModel , std::vector<Entity>> &entities)
{
    for (const auto& modelPair : entities) {
        auto texturedModel = modelPair.first;
        auto rawModel = texturedModel.getRawModel();
        
        // 绑定模型
        bindModel(rawModel);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texturedModel.getTexture().getID());
        if(texturedModel.getTexture().isHasTransparency())
        {
            glDisable(GL_CULL_FACE);
        }
        // 渲染使用该模型的所有实体
        for (const auto& entity : modelPair.second) {
            prepareInstance(entity);
            glDrawElements(GL_TRIANGLES, rawModel.getVertexCount(), GL_UNSIGNED_INT, 0);
        }
        if(texturedModel.getTexture().isHasTransparency())
        {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
        }
    }
    
    // 清理OpenGL状态
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindVertexArray(0);
}

void ShadowMapEntityRenderer::bindModel(const RawModel &rawModel)
{
    // 绑定VAO并启用位置属性（属性0）
    glBindVertexArray(rawModel.getVaoID());
    glEnableVertexAttribArray(0); // 位置属性
    glEnableVertexAttribArray(1);
}

void ShadowMapEntityRenderer::prepareInstance(const Entity& entity) {
    // 创建模型变换矩阵
    glm::mat4 modelMatrix = Maths::createTransformationMatrix(
        entity.getPosition(),
        entity.getRotX(),
        entity.getRotY(),
        entity.getRotZ(),
        entity.getScale()
    );
    
    // 计算MVP矩阵：投影视图矩阵 × 模型矩阵
    glm::mat4 mvpMatrix = projectionViewMatrix * modelMatrix;
    
    // 加载MVP矩阵到着色器
    shader.loadMvpMatrix(mvpMatrix);
}