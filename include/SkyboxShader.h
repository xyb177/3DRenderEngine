#pragma once
#include <string>
#include "ShaderProgram.h"
#include "Camera.h"
#include "Maths.h"
#include "DisplayManager.h"

class SkyboxShader : public ShaderProgram
{
public:
    SkyboxShader();
    void loadViewMatrix(const Camera& camera);
    void loadProjectionMatrix(const glm::mat4& projection);
    void loadFogColour(float r, float g ,float b);
    void loadBlendFactor(float blend);
    void connectTextureUnits();
protected:
    void getAllUniformLocations() override;

private:
    int location_projectionMatrix;
    int location_viewMatrix;
    int location_fogColour;
    int location_cubeMap;
    int location_cubeMap2;
    int location_blendFactor;

    float rotation = 0;

    static const std::string VERTEX_FILE;
    static const std::string FRAGMENT_FILE;
    static constexpr float  ROTATE_SPEED = 1.0f;
};