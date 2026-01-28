#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include <memory>
#include "Camera.h"

/**
 * 表示世界中物体将投射阴影的3D立方体区域
 * 每帧更新以优化区域，使其尽可能小（以获得最佳阴影贴图分辨率）
 * 同时确保不会太小而避免物体在应该投射阴影时没有阴影
 */
class ShadowBox {


public:
    /**
     * 创建新的阴影盒并计算与相机视锥体相关的初始值
     * @param lightViewMatrix 光的"视图矩阵"
     * @param camera 游戏中的相机
     */
    ShadowBox(const glm::mat4& lightViewMatrix, Camera& camera);
    
    /**
     * 根据光的方向和相机的视锥体更新阴影盒的边界
     */
    void update();
    
    /**
     * 获取"视图立方体"在世界空间中的中心点
     */
    glm::vec3 getCenter() const;
    
    // 获取尺寸方法
    float getWidth() const { return maxX - minX; }
    float getHeight() const { return maxY - minY; }
    float getLength() const { return maxZ - minZ; }
    
    void setLightViewMatrix(glm::mat4& matrix) {lightViewMatrix = matrix;  }
private:
    // 计算视锥体顶点
    std::vector<glm::vec4> calculateFrustumVertices(const glm::mat4& rotation, 
                                                   const glm::vec3& forwardVector,
                                                   const glm::vec3& centerNear,
                                                   const glm::vec3& centerFar);
    
    // 计算光空间中的视锥体角点
    glm::vec4 calculateLightSpaceFrustumCorner(const glm::vec3& startPoint,
                                              const glm::vec3& direction,
                                              float width);
    
    // 计算相机旋转矩阵
    glm::mat4 calculateCameraRotationMatrix() const;
    
    // 计算近平面和远平面的宽度和高度
    void calculateWidthsAndHeights();
    
    // 获取显示器的宽高比
    float getAspectRatio() const;

private:
    static constexpr float OFFSET = 10.0f;
    static constexpr float SHADOW_DISTANCE = 150.0f;
    
    static const glm::vec4 UP;
    static const glm::vec4 FORWARD;

    float minX, maxX;
    float minY, maxY;
    float minZ, maxZ;
    
    glm::mat4 lightViewMatrix;
    Camera& camera;
    
    float farHeight, farWidth, nearHeight, nearWidth;
};
