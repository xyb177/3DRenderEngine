#pragma once
#include "glm.hpp"
#include <vector>
#include <memory>
#include "RawModel.h"
#include "Loader.h"
#include "ContrastChanger.h"
#include "HorizontalBlur.h"
#include "VerticalBlur.h"
#include "BrightFilter.h"
#include "CombineFilter.h"

/**
 * @brief 后期处理管理器
 * 负责初始化和管理后期处理所需的资源
 */
class PostProcessing {
public:
    static PostProcessing& getInstance();
    
    /**
     * @brief 初始化后期处理资源
     * @param loader 模型加载器引用
     */
    void init(Loader& loader);
    
    /**
     * @brief 执行后期处理
     * @param colourTexture 输入颜色纹理ID
     */
    void doPostProcessing(GLuint colourTexture);
    
    /**
     * @brief 清理资源
     * 释放所有OpenGL资源
     */
    void cleanUp();

private:
    
    PostProcessing() = default;
    /**
     * @brief 开始后期处理渲染
     * 设置渲染状态和绑定VAO
     */
    void start();
    
    /**
     * @brief 结束后期处理渲染
     * 恢复渲染状态
     */
    void end();
private:
    RawModel quad{0,0}; // 全屏四边形模型
    ContrastChanger contrastChanger;
    HorizontalBlur* hBlur;
    VerticalBlur* vBlur;
    BrightFilter* brightFilter;
    CombineFilter* combineFilter;
    static const std::vector<float> POSITIONS; // 顶点位置数据
    static PostProcessing* instance;
};

