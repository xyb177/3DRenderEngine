#pragma once

#include "Entity.h"
#include "TexturedModel.h"
#include "RawModel.h"
#include "ShadowShader.h"
#include "glm.hpp"
#include <unordered_map>
#include <vector>
#include <memory>

/**
 * 阴影映射实体渲染器
 * 负责将实体渲染到阴影贴图中
 * 使用模型分组优化渲染性能
 */
class ShadowMapEntityRenderer {

public:
    /**
     * 构造函数
     * @param shader 阴影着色器程序
     * @param projectionViewMatrix 正交投影矩阵与光源视图矩阵的乘积
     */
    ShadowMapEntityRenderer(ShadowShader& shader, const glm::mat4& projectionViewMatrix);

    /**
     * 渲染实体到阴影贴图
     * 按模型分组渲染，优化性能
     * @param entities 要渲染的实体映射表（按纹理模型分组）
     */
    
    void render(const std::unordered_map<TexturedModel, std::vector<Entity>>& entities);

    void setProjectionViewMatrix(glm::mat4& matrix) { projectionViewMatrix = matrix;}
private:
    /**
     * 绑定模型准备渲染
     * @param rawModel 原始模型数据
     */
    void bindModel(const RawModel& rawModel);

    /**
     * 准备实体实例渲染
     * @param entity 要渲染的实体
     */
    void prepareInstance(const Entity& entity);
private:
    glm::mat4 projectionViewMatrix; // 投影视图矩阵
    ShadowShader shader;            // 阴影着色器


};