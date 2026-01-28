#pragma once
#include <memory>
#include "glm.hpp"
#include "Fbo.h"

/**
 * @brief 图像渲染器类，用于处理离屏渲染和后期处理
 * 负责管理FBO（帧缓冲对象）并渲染全屏四边形
 */
class ImageRenderer {

public:
    /**
     * @brief 构造函数 - 创建指定尺寸的FBO
     * @param width 渲染目标宽度
     * @param height 渲染目标高度
     */
    ImageRenderer(int width, int height);
    
    /**
     * @brief 默认构造函数 - 不使用FBO（直接渲染到屏幕）
     */
    ImageRenderer();
    
    
    /**
     * @brief 渲染全屏四边形
     * 如果存在FBO则绑定到FBO，否则渲染到默认帧缓冲
     */
    void renderQuad();
    
    /**
     * @brief 获取输出纹理ID（如果使用FBO）
     * @return FBO的颜色附件纹理ID
     */
    unsigned int getOutputTexture() const;
    
    /**
     * @brief 清理资源
     * 释放FBO和相关OpenGL资源
     */
    void cleanUp();
    

private:
   Fbo* fbo; 

};

