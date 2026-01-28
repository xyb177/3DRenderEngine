#pragma once 
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include "glm.hpp"

#include <cstdint>
#include <vector>
/**
 * @brief 帧缓冲对象（FBO）类
 * 用于离屏渲染，支持颜色纹理附件和不同类型的深度缓冲附件
 */
class Fbo {
public:
    /**
     * @brief 构造函数
     * @param width FBO宽度
     * @param height FBO高度
     * @param depthBufferType 深度缓冲类型
     */
    Fbo(int width, int height, int depthBufferType);
    

    Fbo(int width, int height);
    /**
     * @brief 清理所有OpenGL资源
     */
    void cleanUp();

    /**
     * @brief 绑定帧缓冲为绘制目标
     */
    void bindFrameBuffer();

    /**
     * @brief 解绑帧缓冲（绑定到默认帧缓冲）
     */
    void unbindFrameBuffer();

    /**
     * @brief 绑定帧缓冲为读取目标
     */
    void bindToRead();

    /**
     * @brief 获取颜色纹理ID
     * @return 颜色纹理ID
     */
    unsigned int getColourTexture() const { return colourTexture; }

    /**
     * @brief 获取深度纹理ID
     * @return 深度纹理ID
     */
    unsigned int getDepthTexture() const { return depthTexture; }

    unsigned int getFrameBuffer() const {return frameBuffer;}
    /**
     * @brief 获取FBO宽度
     * @return 宽度
     */
    int getWidth() const { return width; }

    /**
     * @brief 获取FBO高度
     * @return 高度
     */
    int getHeight() const { return height; }

    void resolveToFbo(int readBuffer, Fbo& outputFbo);

    void resolveToScreen();
   
public:
    static constexpr int NONE = 0;
	static constexpr int DEPTH_TEXTURE = 1;
	static constexpr int DEPTH_RENDER_BUFFER = 2;
private:
    /**
     * @brief 初始化帧缓冲对象
     * @param type 深度缓冲附件类型
     */
    void initialiseFrameBuffer(int type);

    /**
     * @brief 创建帧缓冲对象
     */
    void createFrameBuffer();

    /**
     * @brief 创建颜色纹理附件
     */
    void createTextureAttachment();

    /**
     * @brief 创建深度纹理附件
     */
    void createDepthTextureAttachment();



    unsigned int createMultisampleColourAttachment(int attachment);
    /**
     * @brief 创建深度渲染缓冲附件
     */
    void createDepthBufferAttachment();

    void determineDrawBuffers();
private:
    const int width;            // FBO宽度
    const int height;           // FBO高度

    unsigned int frameBuffer;         // 帧缓冲对象ID
    unsigned int colourTexture;       // 颜色纹理ID
    unsigned int depthTexture;        // 深度纹理ID
    unsigned int depthBuffer;         // 深度渲染缓冲ID
    unsigned int colourBuffer;        // 颜色渲染缓冲ID
    unsigned int colourBuffer2;
    bool multisampleAndMultiTarget = false;

};
