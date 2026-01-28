#include "Fbo.h"
#include <stdexcept>
#include <iostream>

Fbo::Fbo(int width, int height, int depthBufferType)
    : width(width), height(height)
{
    
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("FBO dimensions must be positive");
    }
    
    initialiseFrameBuffer(depthBufferType);
}

Fbo::Fbo(int width, int height)
    : width(width), height(height),multisampleAndMultiTarget(true)
{
    initialiseFrameBuffer(DEPTH_RENDER_BUFFER);
}

void Fbo::cleanUp() {
    if (frameBuffer != 0) {
        glDeleteFramebuffers(1, &frameBuffer);
        frameBuffer = 0;
    }
    if (colourTexture != 0) {
        glDeleteTextures(1, &colourTexture);
        colourTexture = 0;
    }
    if (depthTexture != 0) {
        glDeleteTextures(1, &depthTexture);
        depthTexture = 0;
    }
    if (depthBuffer != 0) {
        glDeleteRenderbuffers(1, &depthBuffer);
        depthBuffer = 0;
    }
    if (colourBuffer != 0) {
        glDeleteRenderbuffers(1, &colourBuffer);
        colourBuffer = 0;
    }

    if (colourBuffer2 != 0) {
        glDeleteRenderbuffers(1, &colourBuffer2);
        colourBuffer2 = 0;
    }
}

void Fbo::resolveToFbo(int readBuffer, Fbo& outputFbo)
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER,outputFbo.getFrameBuffer());
    glBindFramebuffer(GL_READ_FRAMEBUFFER,frameBuffer);
    glReadBuffer(readBuffer);
    glBlitFramebuffer(0,0,width,height,0,0,outputFbo.getWidth(),outputFbo.getHeight(),
                        GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT,GL_NEAREST);
    unbindFrameBuffer();
}

void Fbo::resolveToScreen()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER,frameBuffer);
    glDrawBuffer(GL_BACK);
    int width1, height1;
    GLFWwindow* window = glfwGetCurrentContext();
    glfwGetFramebufferSize(window, &width1, &height1); 
    glBlitFramebuffer(0,0,width,height,0,0,width1,height1,
                        GL_COLOR_BUFFER_BIT ,GL_NEAREST);
    unbindFrameBuffer();
}

void Fbo::initialiseFrameBuffer(int type)
{
    createFrameBuffer();
    if(multisampleAndMultiTarget){
        colourBuffer = createMultisampleColourAttachment(GL_COLOR_ATTACHMENT0);
        colourBuffer2 = createMultisampleColourAttachment(GL_COLOR_ATTACHMENT1);
    } else {
        createTextureAttachment();
    }
    
    switch (type) {
        case DEPTH_RENDER_BUFFER:
            createDepthBufferAttachment();
            break;
        case DEPTH_TEXTURE:
            createDepthTextureAttachment();
            break;
        case NONE:
            // 无深度缓冲
            break;
    }
    
    
    unbindFrameBuffer();
}

void Fbo::createFrameBuffer() {
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    determineDrawBuffers();
}

void Fbo::createTextureAttachment() {
    glGenTextures(1, &colourTexture);
    glBindTexture(GL_TEXTURE_2D, colourTexture);
    
    // 创建空的纹理存储
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, 
                 GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    
    // 设置纹理参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    // 附加到帧缓冲
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
                          GL_TEXTURE_2D, colourTexture, 0);
}

void Fbo::createDepthTextureAttachment() {
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    
    // 创建深度纹理
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    
    // 设置纹理参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    // 附加到帧缓冲
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                          GL_TEXTURE_2D, depthTexture, 0);
}

unsigned int  Fbo::createMultisampleColourAttachment(int attachment)
{   
    unsigned int buffer;
    glGenRenderbuffers(1, &buffer);
    glBindRenderbuffer(GL_RENDERBUFFER, buffer); 
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4,GL_RGBA8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, buffer);
    return buffer;
}

void Fbo::createDepthBufferAttachment() {
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    
    if(!multisampleAndMultiTarget){
        // 分配深度缓冲存储
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
    } else{
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4,GL_DEPTH_COMPONENT24, width, height);
    }
    
    
    // 附加到帧缓冲
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                             GL_RENDERBUFFER, depthBuffer);
}

void Fbo::determineDrawBuffers()
{
    std::vector<GLenum> drawBuffers;
    drawBuffers.push_back(GL_COLOR_ATTACHMENT0);
    if(multisampleAndMultiTarget){
        drawBuffers.push_back(GL_COLOR_ATTACHMENT1);
    }
    glDrawBuffers(static_cast<GLsizei>(drawBuffers.size()), drawBuffers.data());
}

void Fbo::bindFrameBuffer()  {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
    glViewport(0, 0, width, height);
}

void Fbo::unbindFrameBuffer()  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    int width, height;
    GLFWwindow* window = glfwGetCurrentContext();
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
}

void Fbo::bindToRead()  {
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuffer);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
}
