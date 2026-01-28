#include "ShadowBox.h"
#include "Camera.h"
#include "MasterRenderer.h"
#include <cmath>
#include <vector>

// 静态常量初始化
const glm::vec4 ShadowBox::UP = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
const glm::vec4 ShadowBox::FORWARD = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);

ShadowBox::ShadowBox(const glm::mat4& lightViewMatrix, Camera& camera)
    : lightViewMatrix(lightViewMatrix), camera(camera) {
    calculateWidthsAndHeights();
}

void ShadowBox::update() {
    glm::mat4 rotation = calculateCameraRotationMatrix();
    glm::vec3 forwardVector = glm::vec3(rotation * FORWARD);
    
    glm::vec3 toFar = forwardVector * SHADOW_DISTANCE;
    glm::vec3 toNear = forwardVector * MasterRenderer::NEAR_PLANE;
    glm::vec3 centerNear = toNear + camera.getPosition();
    glm::vec3 centerFar = toFar + camera.getPosition();
    
    auto points = calculateFrustumVertices(rotation, forwardVector, centerNear, centerFar);
    
    bool first = true;
    for (const auto& point : points) {
        if (first) {
            minX = maxX = point.x;
            minY = maxY = point.y;
            minZ = maxZ = point.z;
            first = false;
            continue;
        }
        
        minX = std::min(minX, point.x);
        maxX = std::max(maxX, point.x);
        minY = std::min(minY, point.y);
        maxY = std::max(maxY, point.y);
        minZ = std::min(minZ, point.z);
        maxZ = std::max(maxZ, point.z);
    }
    maxZ += OFFSET;
}

glm::vec3 ShadowBox::getCenter() const {
    float x = (minX + maxX) / 2.0f;
    float y = (minY + maxY) / 2.0f;
    float z = (minZ + maxZ) / 2.0f;
    
    glm::vec4 cen(x, y, z, 1.0f);
    glm::mat4 invertedLight = glm::inverse(lightViewMatrix);
    return glm::vec3(invertedLight * cen);
}

std::vector<glm::vec4> ShadowBox::calculateFrustumVertices(const glm::mat4& rotation,
                                                         const glm::vec3& forwardVector,
                                                         const glm::vec3& centerNear,
                                                         const glm::vec3& centerFar) {
    glm::vec3 upVector = glm::vec3(rotation * UP);
    glm::vec3 rightVector = glm::cross(forwardVector, upVector);
    glm::vec3 downVector = -upVector;
    glm::vec3 leftVector = -rightVector;
    
    glm::vec3 farTop = centerFar + (upVector * farHeight);
    glm::vec3 farBottom = centerFar + (downVector * farHeight);
    glm::vec3 nearTop = centerNear + (upVector * nearHeight);
    glm::vec3 nearBottom = centerNear + (downVector * nearHeight);
    
    std::vector<glm::vec4> points(8);
    points[0] = calculateLightSpaceFrustumCorner(farTop, rightVector, farWidth);
    points[1] = calculateLightSpaceFrustumCorner(farTop, leftVector, farWidth);
    points[2] = calculateLightSpaceFrustumCorner(farBottom, rightVector, farWidth);
    points[3] = calculateLightSpaceFrustumCorner(farBottom, leftVector, farWidth);
    points[4] = calculateLightSpaceFrustumCorner(nearTop, rightVector, nearWidth);
    points[5] = calculateLightSpaceFrustumCorner(nearTop, leftVector, nearWidth);
    points[6] = calculateLightSpaceFrustumCorner(nearBottom, rightVector, nearWidth);
    points[7] = calculateLightSpaceFrustumCorner(nearBottom, leftVector, nearWidth);
    
    return points;
}

glm::vec4 ShadowBox::calculateLightSpaceFrustumCorner(const glm::vec3& startPoint,
                                                     const glm::vec3& direction,
                                                     float width) {
    glm::vec3 point = startPoint + (direction * width);
    glm::vec4 point4f(point.x, point.y, point.z, 1.0f);
    return lightViewMatrix * point4f;
}

glm::mat4 ShadowBox::calculateCameraRotationMatrix() const {
    glm::mat4 rotation(1.0f);
    rotation = glm::rotate(rotation, glm::radians(camera.getYaw()), glm::vec3(0.0f, 1.0f, 0.0f));
    rotation = glm::rotate(rotation, glm::radians(camera.getPitch()), glm::vec3(1.0f, 0.0f, 0.0f));
    return rotation;
}

void ShadowBox::calculateWidthsAndHeights() {
    farWidth = static_cast<float>(SHADOW_DISTANCE * std::tan(glm::radians(MasterRenderer::FOV)));
    nearWidth = static_cast<float>(MasterRenderer::NEAR_PLANE * std::tan(glm::radians(MasterRenderer::FOV)));
    farHeight = farWidth / getAspectRatio();
    nearHeight = nearWidth / getAspectRatio();
}

float ShadowBox::getAspectRatio() const {
    int width, height;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
    return static_cast<float>(width) / static_cast<float>(height);
}
