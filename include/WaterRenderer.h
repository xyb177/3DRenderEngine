#pragma once
#include <vector>
#include "glm.hpp"
#include "RawModel.h"
#include "Loader.h"
#include "WaterShader.h"
#include "Camera.h"
#include "Maths.h"
#include "WaterTile.h"
#include "WaterFrameBuffers.h"
class WaterRenderer {
public:
    WaterRenderer(Loader& loader, WaterShader& shader, const glm::mat4& projectionMatrix, WaterFrameBuffers& fbos);
    
    void render(const std::vector<WaterTile>& water, const Camera& camera, const Light& sun);

private:
    void prepareRender(const Camera& camera, const Light& sun);
    void unbind();
    void setUpVAO(Loader& loader);

    RawModel quad{0,0};
    WaterShader& shader;
    WaterFrameBuffers& fbos;

    static std::string DUDV_MAP;
    static std::string NORMAL_MAP;
    int dudvTexture;
    int normalMap;
    float WAVE_SPEED = 0.03; 
    float moveFactor = 0;
};