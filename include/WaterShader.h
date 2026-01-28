#pragma once
#include "glm.hpp"
#include "ShaderProgram.h"
#include "Camera.h"
#include "Maths.h"
#include "Light.h"
class WaterShader : public ShaderProgram {
public:
    WaterShader();
    
    void loadProjectionMatrix(const glm::mat4& projection);
    void loadViewMatrix(const Camera& camera);
    void loadModelMatrix(const glm::mat4& modelMatrix);
    void connectTextureUnits();
    void loadMoveFactor(float movefactor);
    void loadLight(const Light& sun);

protected:
   
    void getAllUniformLocations() override;

private:
    int location_modelMatrix;
    int location_viewMatrix;
    int location_projectionMatrix;
    int location_reflectionTexture;
    int location_refractionTexture;
    int location_dudvMap;
    int location_moveFactor;
    int location_cameraPosition;
    int location_normalMap;
    int location_lightColour;
    int location_lightPosition;
    int location_depthMap;

    static const std::string VERTEX_FILE;
    static const std::string FRAGMENT_FILE;
};
