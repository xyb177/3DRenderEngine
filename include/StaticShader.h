#pragma once
#include "ShaderProgram.h"
#include <string>
#include "Camera.h"
#include "Light.h"
class StaticShader : public ShaderProgram {
public:
    StaticShader();
    void loadTransformationMatrix(const glm::mat4& matrix);
    void loadViewMatrix(const Camera& camera);
    void loadProjectionMatrix(const glm::mat4& projection);
    void loadLight(const std::vector<Light>& light);
    void loadShineVariables(float damper, float reflectivity);
    void loadFakeLightingVariable(bool useFake);
    void loadSkyColour(float r, float g, float b);
    void loadNumberOfRow(int n);
    void loadOffset(float x, float y);
    void loadClipPlane(glm::vec4 plane);
    void connectTextureUnits();
    void loadUseSpecularMap(bool useMap);
    void loadUseEnvironmentMap(bool useMap, float strength);
protected:
    void getAllUniformLocations() override;

private:
    unsigned int location_transformationMatrix;
    unsigned int location_projectionMatrix;
    unsigned int location_viewMatrix;
    unsigned int location_cameraPosition;
    std::vector <unsigned int> location_lightPosition;
    std::vector <unsigned int> location_lightColour;
    std::vector <unsigned int> location_attenuation;
    unsigned int location_shineDamper;
    unsigned int location_reflectivity;
    unsigned int location_useFakeLighting;
    unsigned int location_skyColour;
    unsigned int location_numberOfRows;
    unsigned int location_offset;
    unsigned int location_plane;
    unsigned int location_specularMap;
    unsigned int location_useSpecularMap;
    unsigned int location_modelTexture;
    unsigned int location_environmentMap;
    unsigned int location_useEnvironmentMap;
    unsigned int location_reflectionStrength;

    static const std::string VERTEX_FILE;
    static const std::string FRAGMENT_FILE;
    static constexpr int MAX_LIGHTS = 4;
};
