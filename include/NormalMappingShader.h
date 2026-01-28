#pragma once

#include <vector>
#include "glm.hpp"
#include "ShaderProgram.h"
#include "Light.h"

class NormalMappingShader : public ShaderProgram {
public:
    NormalMappingShader();
    
    void connectTextureUnits();
    void loadClipPlane(const glm::vec4& plane);
    void loadNumberOfRows(int numberOfRows);
    void loadOffset(float x, float y);
    void loadSkyColour(float r, float g, float b);
    void loadShineVariables(float damper, float reflectivity);
    void loadTransformationMatrix(const glm::mat4& matrix);
    void loadLights(const std::vector<Light>& lights, const glm::mat4& viewMatrix);
    void loadViewMatrix(const glm::mat4& viewMatrix);
    void loadProjectionMatrix(const glm::mat4& projection);
    void loadUseSpecularMap(bool useMap);
protected:

    void getAllUniformLocations() override;

private:
    int location_transformationMatrix;
    int location_projectionMatrix;
    int location_viewMatrix;
    std::vector<int> location_lightPositionEyeSpace;
    std::vector<int> location_lightColour;
    std::vector<int> location_attenuation;
    int location_shineDamper;
    int location_reflectivity;
    int location_skyColour;
    int location_numberOfRows;
    int location_offset;
    int location_plane;
    int location_modelTexture;
    int location_normalMap;
    int location_specularMap;
    int location_useSpecularMap;

    glm::vec3 getEyeSpacePosition(const Light& light, const glm::mat4& viewMatrix);

    static const std::string VERTEX_FILE;
    static const std::string FRAGMENT_FILE;
    static constexpr int MAX_LIGHTS = 4 ;

};
