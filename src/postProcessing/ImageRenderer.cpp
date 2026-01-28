#include "ImageRenderer.h"
#include "Fbo.h" // 假设Fbo类已实现
#include <glad/glad.h>

ImageRenderer::ImageRenderer(int width, int height) 
{
    fbo = new Fbo(width, height, Fbo::NONE);
}

ImageRenderer::ImageRenderer() : fbo(nullptr) {
}

void ImageRenderer::renderQuad() {
    // 绑定FBO（如果存在）
    if (fbo != nullptr) {
        fbo->bindFrameBuffer();
    }
    
    // 清除颜色缓冲
    glClear(GL_COLOR_BUFFER_BIT);
    
    // 渲染全屏四边形（三角形带方式）
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    // 解绑FBO（如果存在）
    if (fbo != nullptr) {
        fbo->unbindFrameBuffer();
    }
}

unsigned int ImageRenderer::getOutputTexture() const {
    if (fbo != nullptr) {
        return fbo->getColourTexture();
    }
    return 0; // 返回0表示没有纹理（直接渲染到屏幕）
}

void ImageRenderer::cleanUp() {
    if (fbo != nullptr) {
        fbo->cleanUp();
    }
}

