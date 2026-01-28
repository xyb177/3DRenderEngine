#pragma once 
#include "glm.hpp"
#include <memory>

/**
 * 阴影渲染的帧缓冲区类
 * 设置深度纹理，在阴影渲染过程中可以渲染到该纹理，生成阴影贴图
 */
class ShadowFrameBuffer {


public:
    /**
     * 初始化指定大小的帧缓冲区和阴影贴图
     * @param width - 阴影贴图的宽度（像素）
     * @param height - 阴影贴图的高度（像素）
     */
    ShadowFrameBuffer(int width, int height);
    
    /**
     * 析构函数，游戏关闭时删除帧缓冲区和阴影贴图纹理
     */
    ~ShadowFrameBuffer();

    /**
     * 绑定帧缓冲区，将其设置为当前渲染目标
     */
    void bindFrameBuffer();
    
    /**
     * 解绑帧缓冲区，将默认帧缓冲区设置为当前渲染目标
     */
    void unbindFrameBuffer();
    
    /**
     * 获取阴影贴图纹理ID
     * @return 阴影贴图纹理ID
     */
    unsigned int getShadowMap() const;

private:
    const int WIDTH;
    const int HEIGHT;
    unsigned int fbo;
    unsigned int shadowMap;

    // 私有方法
    void initialiseFrameBuffer();
    void bindFrameBuffer(unsigned int frameBuffer, int width, int height);
    unsigned int createFrameBuffer();
    unsigned int createDepthBufferAttachment(int width, int height);
};

