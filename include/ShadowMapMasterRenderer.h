#pragma once

#include "ShadowFrameBuffer.h"
#include "ShadowShader.h"
#include "ShadowBox.h"
#include "ShadowMapEntityRenderer.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include <unordered_map>
#include <vector>
#include <memory>
#include <cmath>
#include "Light.h"
#include "Entity.h"


/**
 * 阴影映射主渲染器
 * 负责管理整个阴影渲染过程，包括阴影贴图的生成和渲染
 * 这是阴影包中唯一需要从外部引用的类
 */
class ShadowMapMasterRenderer {


public:
    /**
     * 构造函数
     * @param camera 场景中使用的相机
     */
    ShadowMapMasterRenderer(Camera& camera);

    /**
     * 执行阴影渲染过程，将实体渲染到阴影贴图
     * @param entities 要渲染的实体映射表（按纹理模型分组）
     * @param sun 场景中的太阳光源
     */
    void render(const std::unordered_map<TexturedModel, std::vector<Entity>>& entities, const Light& sun);

    /**
     * 获取到阴影贴图空间的偏置投影视图矩阵
     * 用于在主渲染过程中将片段转换到阴影贴图空间
     * @return 到阴影贴图空间的矩阵
     */
    glm::mat4 getToShadowMapSpaceMatrix() const;

    /**
     * 清理资源
     */
    void cleanUp();

    /**
     * 获取阴影贴图纹理ID
     * @return 阴影贴图纹理ID
     */
    int getShadowMap() const;

    /**
     * 获取光源空间变换矩阵
     * @return 光源视图矩阵
     */
    glm::mat4 getLightSpaceTransform() const;
private:
    /**
     * 准备阴影渲染过程
     * @param lightDirection 光源方向
     * @param box 阴影盒子
     */
    void prepare(const glm::vec3& lightDirection, const ShadowBox& box);

    /**
     * 完成阴影渲染过程
     */
    void finish();

    /**
     * 更新光源视图矩阵
     * @param direction 光源方向
     * @param center 视锥体中心
     */
    void updateLightViewMatrix(const glm::vec3& direction, const glm::vec3& center);

    /**
     * 更新正交投影矩阵
     * @param width 阴影盒子宽度
     * @param height 阴影盒子高度
     * @param length 阴影盒子长度
     */
    void updateOrthoProjectionMatrix(float width, float height, float length);

    /**
     * 创建阴影贴图空间转换偏移矩阵
     * @return 偏移矩阵
     */
    static glm::mat4 createOffset();
private:

    static constexpr int SHADOW_MAP_SIZE = 4096; // 阴影贴图尺寸

    glm::mat4 projectionMatrix;            // 正交投影矩阵
    glm::mat4 lightViewMatrix;             // 光源视图矩阵
    glm::mat4 projectionViewMatrix;        // 投影视图矩阵
    glm::mat4 offset;                      // 阴影贴图空间转换偏移矩阵

    ShadowFrameBuffer shadowFbo;           // 阴影帧缓冲区
    ShadowShader shader;                   // 阴影着色器
    ShadowBox shadowBox;                   // 阴影盒子，计算视锥体
    
    ShadowMapEntityRenderer entityRenderer; // 实体渲染器

    
};
