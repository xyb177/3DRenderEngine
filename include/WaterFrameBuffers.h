#pragma once
#include "glad/glad.h"
#include "glm.hpp"
#include <GLFW/glfw3.h>
class WaterFrameBuffers {
public:
    static constexpr int REFLECTION_WIDTH = 320;
    static constexpr int REFLECTION_HEIGHT = 180;
    static constexpr int REFRACTION_WIDTH = 1280;
    static constexpr int REFRACTION_HEIGHT = 720;

    WaterFrameBuffers();
    ~WaterFrameBuffers();

    void cleanUp();
    void bindReflectionFrameBuffer();
    void bindRefractionFrameBuffer();
    void unbindCurrentFrameBuffer();

    unsigned int getReflectionTexture() const;
    unsigned int getRefractionTexture() const;
    unsigned int getRefractionDepthTexture() const;

private:
    void initialiseReflectionFrameBuffer();
    void initialiseRefractionFrameBuffer();
    void bindFrameBuffer(unsigned int frameBuffer, int width, int height);
    
    unsigned int createFrameBuffer();
    unsigned int createTextureAttachment(int width, int height);
    unsigned int createDepthTextureAttachment(int width, int height);
    unsigned int createDepthBufferAttachment(int width, int height);

    unsigned int reflectionFrameBuffer;
    unsigned int reflectionTexture;
    unsigned int reflectionDepthBuffer;
    
    unsigned int refractionFrameBuffer;
    unsigned int refractionTexture;
    unsigned int refractionDepthTexture;
};
