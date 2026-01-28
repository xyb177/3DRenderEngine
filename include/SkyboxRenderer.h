#pragma once
#include "glm.hpp"
#include "RawModel.h"
#include "SkyboxShader.h"
#include "Loader.h"
#include "Camera.h"
class SkyBoxRenderer{

public:
    SkyBoxRenderer(Loader& loader, const glm::mat4& projectionMatrix);

    void render(const Camera& camera, float r, float g ,float b);
private:
    const static constexpr float SIZE = 500.0f;
    const static  std::vector<float> vertices; 

    std::vector<std::string> TEXTURE_FILES = {
        "res/Skybox/right.png",
        "res/Skybox/left.png",
        "res/Skybox/top.png",
        "res/Skybox/bottom.png",
        "res/Skybox/front.png",
        "res/Skybox/back.png"
    };
    
    std::vector<std::string> NIGHT_TEXTURE_FILES = {
        "res/Skybox/nightRight.png",
        "res/Skybox/nightLeft.png",
        "res/Skybox/nightTop.png",
        "res/Skybox/nightBottom.png",
        "res/Skybox/nightFront.png",
        "res/Skybox/nightBack.png"
    };

    RawModel cube;
    int texture;
    int nightTexture;
    SkyboxShader shader;
    float time = 0;
    void bindTextures();
};