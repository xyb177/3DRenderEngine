#include "ShadowFrameBuffer.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

ShadowFrameBuffer::ShadowFrameBuffer(int width, int height) 
    : WIDTH(width), HEIGHT(height) {
    initialiseFrameBuffer();
}

ShadowFrameBuffer::~ShadowFrameBuffer() {
    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(1, &shadowMap);
}

void ShadowFrameBuffer::bindFrameBuffer() {
    bindFrameBuffer(fbo, WIDTH, HEIGHT);
}

void ShadowFrameBuffer::unbindFrameBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    // 获取当前窗口大小并设置视口
    int width, height;
    GLFWwindow* window = glfwGetCurrentContext();
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
}

unsigned int ShadowFrameBuffer::getShadowMap() const {
    return shadowMap;
}

void ShadowFrameBuffer::initialiseFrameBuffer() {
    fbo = createFrameBuffer();
    shadowMap = createDepthBufferAttachment(WIDTH, HEIGHT);
    unbindFrameBuffer();
}

void ShadowFrameBuffer::bindFrameBuffer(unsigned int frameBuffer, int width, int height) {
    glBindTexture(GL_TEXTURE_2D, 0); // 解绑当前纹理
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
    glViewport(0, 0, width, height);
}

unsigned int ShadowFrameBuffer::createFrameBuffer() {
    unsigned int frameBuffer;
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    
    // 设置无颜色缓冲区渲染
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    
    return frameBuffer;
}

unsigned int ShadowFrameBuffer::createDepthBufferAttachment(int width, int height) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    // 创建深度纹理
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    
    // 设置纹理参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    // 将纹理附加到帧缓冲区的深度附件
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);
    
    // 检查帧缓冲区完整性
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Error: Framebuffer is not complete!" << std::endl;
    }
    
    return texture;
}
