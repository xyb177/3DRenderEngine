#pragma once
#include <string>
#include "ShaderProgram.h"
#include "Light.h"
#include "Camera.h"
#include "Maths.h"

class TerrainShader : public ShaderProgram {
public:
    TerrainShader(); 
    
    void loadShineVariables(float damper, float reflectivity);
    void loadTransformationMatrix(const glm::mat4& matrix);
    void loadLight(const std::vector<Light>& light);
    void loadViewMatrix(const Camera& camera);
    void loadProjectionMatrix(const glm::mat4& projection);
    void loadSkyColour(float r, float g, float b);
    void loadClipPlane(glm::vec4 plane);
    void loadToShadowSpaceMatrix(glm::mat4 matrix);
    void connectTextureUnits();
protected:
    
    void getAllUniformLocations() override;

private:
    int location_transformationMatrix;
    int location_projectionMatrix;
    int location_viewMatrix;
    std::vector<int> location_lightPosition;
    std::vector<int> location_lightColour;
    std::vector <unsigned int> location_attenuation;
    int location_shineDamper;
    int location_reflectivity;
    int location_skyColour;
    int location_backgroundTexture;
    int location_rTexture;
    int location_gTexture;
    int location_bTexture;
    int location_blendMap;
    int location_plane;
    int location_toShadowMapSpace;
    int location_shadowMap;

    static const std::string VERTEX_FILE;
    static const std::string FRAGMENT_FILE;
    static constexpr int MAX_LIGHTS = 4 ;
};
