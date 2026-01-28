#include "ShadowMapMasterRenderer.h"


ShadowMapMasterRenderer::ShadowMapMasterRenderer(Camera& camera)
    : projectionMatrix(glm::mat4(1.0f)),
      lightViewMatrix(glm::mat4(1.0f)),
      projectionViewMatrix(glm::mat4(1.0f)),
      shadowBox(glm::mat4(1.0f), camera),
      shadowFbo(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE),
      shader(),
      entityRenderer(shader, glm::mat4(1.0f)),
      offset(createOffset()) {
      

    shadowBox.setLightViewMatrix(lightViewMatrix);
    entityRenderer.setProjectionViewMatrix(projectionViewMatrix);  
}

void ShadowMapMasterRenderer::render(const std::unordered_map<TexturedModel, std::vector<Entity>>& entities, const Light& sun) {
    // 更新阴影盒子
    shadowBox.update();
    
    // 获取光源位置并计算方向
    glm::vec3 sunPosition = sun.getPosition();
    glm::vec3 lightDirection = -sunPosition; // 光源方向为光源位置的负方向
    
    // 准备渲染并渲染实体
    prepare(lightDirection, shadowBox);
    entityRenderer.render(entities);
    finish();
}

glm::mat4 ShadowMapMasterRenderer::getToShadowMapSpaceMatrix() const {
    return offset * projectionViewMatrix;
}

void ShadowMapMasterRenderer::cleanUp() {
    shader.cleanUp();
    //shadowFbo.cleanUp();
}

int ShadowMapMasterRenderer::getShadowMap() const {
    return shadowFbo.getShadowMap();
}

glm::mat4 ShadowMapMasterRenderer::getLightSpaceTransform() const {
    return lightViewMatrix;
}

void ShadowMapMasterRenderer::prepare(const glm::vec3& lightDirection, const ShadowBox& box) {
    // 更新投影和视图矩阵
    updateOrthoProjectionMatrix(box.getWidth(), box.getHeight(), box.getLength());
    updateLightViewMatrix(lightDirection, box.getCenter());
    
    // 计算投影视图矩阵
    projectionViewMatrix = projectionMatrix * lightViewMatrix;
    
    shadowBox.setLightViewMatrix(lightViewMatrix);
    entityRenderer.setProjectionViewMatrix(projectionViewMatrix);  
    // 绑定帧缓冲区并设置OpenGL状态
    shadowFbo.bindFrameBuffer();
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);
    
    // 启动着色器
    shader.start();
}

void ShadowMapMasterRenderer::finish() {
    shader.stop();
    shadowFbo.unbindFrameBuffer();
}

void ShadowMapMasterRenderer::updateLightViewMatrix(const glm::vec3& direction, const glm::vec3& center) {
    glm::vec3 normalizedDir = glm::normalize(direction);
    glm::vec3 negatedCenter = -center;
    
    // 重置为单位矩阵
    lightViewMatrix = glm::mat4(1.0f);
    
    // 计算俯仰角（pitch）
    float pitch = std::acos(glm::length(glm::vec2(normalizedDir.x, normalizedDir.z)));
    
    // 绕X轴旋转（俯仰）
    lightViewMatrix = glm::rotate(lightViewMatrix, pitch, glm::vec3(1.0f, 0.0f, 0.0f));
    
    // 计算偏航角（yaw）
    float yaw = std::atan2(normalizedDir.x ,normalizedDir.z);
    yaw = glm::degrees(yaw);
    yaw = (normalizedDir.z > 0) ? yaw - 180.0f : yaw;
    
    // 绕Y轴旋转（偏航）
    lightViewMatrix = glm::rotate(lightViewMatrix, -glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
    
    // 平移以中心为原点
    lightViewMatrix = glm::translate(lightViewMatrix, negatedCenter);
}

void ShadowMapMasterRenderer::updateOrthoProjectionMatrix(float width, float height, float length) {
    // 创建正交投影矩阵
    projectionMatrix = glm::mat4(1.0f);
    projectionMatrix[0][0] = 2.0f / width;   // X轴缩放
    projectionMatrix[1][1] = 2.0f / height;  // Y轴缩放
    projectionMatrix[2][2] = -2.0f / length; // Z轴缩放（OpenGL Z轴朝里）
    projectionMatrix[3][3] = 1.0f;           // 齐次坐标
}

glm::mat4 ShadowMapMasterRenderer::createOffset() {
    glm::mat4 offset = glm::mat4(1.0f);
    // 先平移 (0.5, 0.5, 0.5)
    offset = glm::translate(offset, glm::vec3(0.5f, 0.5f, 0.5f));
    // 再缩放 (0.5, 0.5, 0.5)
    offset = glm::scale(offset, glm::vec3(0.5f, 0.5f, 0.5f));
    return offset;
}