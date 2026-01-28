#include "NormalMappingShader.h"
#include "glad/glad.h"

const std::string NormalMappingShader::VERTEX_FILE = "src/glsl/normalMapVShader.glsl";
const std::string NormalMappingShader::FRAGMENT_FILE = "src/glsl/normalMapFShader.glsl";

NormalMappingShader::NormalMappingShader() 
                        : ShaderProgram(VERTEX_FILE, FRAGMENT_FILE) { initUniforms(); }

void NormalMappingShader::getAllUniformLocations() {
    location_transformationMatrix = getUniformLocation("transformationMatrix");
    location_projectionMatrix = getUniformLocation("projectionMatrix");
    location_viewMatrix = getUniformLocation("viewMatrix");
    location_shineDamper = getUniformLocation("shineDamper");
    location_reflectivity = getUniformLocation("reflectivity");
    location_skyColour = getUniformLocation("skyColour");
    location_numberOfRows = getUniformLocation("numberOfRows");
    location_offset = getUniformLocation("offset");
    location_plane = getUniformLocation("plane");
    location_modelTexture = getUniformLocation("modelTexture");
    location_normalMap = getUniformLocation("normalMap");
    
    location_lightPositionEyeSpace.resize(MAX_LIGHTS);
    location_lightColour.resize(MAX_LIGHTS);
    location_attenuation.resize(MAX_LIGHTS);
    for (int i = 0; i < MAX_LIGHTS; i++) {
        location_lightPositionEyeSpace[i] = getUniformLocation("lightPositionEyeSpace[" + std::to_string(i) + "]");
        location_lightColour[i] = getUniformLocation("lightColour[" + std::to_string(i) + "]");
        location_attenuation[i] = getUniformLocation("attenuation[" + std::to_string(i) + "]");
    }

    location_specularMap = getUniformLocation("specularMap");
    location_useSpecularMap = getUniformLocation("useSpecularMap");

    std::cout << "View Matrix Location: " << location_viewMatrix << std::endl;
    std::cout << "Projection Matrix Location: " << location_projectionMatrix << std::endl;
    std::cout << "Transformation Matrix Location: " << location_transformationMatrix << std::endl;
  
    std::cout << "  Light Colour: " << location_lightColour[0] << std::endl;
    std::cout << "  Shine Damper: " << location_shineDamper << std::endl;
    std::cout << "  Reflectivity: " << location_reflectivity << std::endl;
    

    if (location_projectionMatrix == -1 || 
        location_transformationMatrix == -1 || 
        location_viewMatrix == -1) {
        std::cerr << "Error: Failed to find essential matrix uniform locations!" << std::endl;
    }
}

void NormalMappingShader::connectTextureUnits() {
    loadInt(location_modelTexture, 0);
    loadInt(location_normalMap, 1);
    loadInt(location_specularMap,2);
}

void NormalMappingShader::loadClipPlane(const glm::vec4& plane) {
    loadVector(location_plane, plane);
}

void NormalMappingShader::loadNumberOfRows(int numberOfRows) {
    loadFloat(location_numberOfRows, static_cast<float>(numberOfRows));
}

void NormalMappingShader::loadOffset(float x, float y) {
    load2DVector(location_offset, glm::vec2(x, y));
}

void NormalMappingShader::loadSkyColour(float r, float g, float b) {
    loadVector(location_skyColour, glm::vec3(r, g, b));
}

void NormalMappingShader::loadShineVariables(float damper, float reflectivity) {
    loadFloat(location_shineDamper, damper);
    loadFloat(location_reflectivity, reflectivity);
}

void NormalMappingShader::loadTransformationMatrix(const glm::mat4& matrix) {
    loadMatrix(location_transformationMatrix, matrix);
}

void NormalMappingShader::loadLights(const std::vector<Light>& lights, const glm::mat4& viewMatrix) {
    for (int i = 0; i < MAX_LIGHTS; i++) {
        if (i < lights.size()) {
            loadVector(location_lightPositionEyeSpace[i], getEyeSpacePosition(lights[i], viewMatrix));
            loadVector(location_lightColour[i], lights[i].getColour());
            loadVector(location_attenuation[i], lights[i].getAttenuation());
        } else {
            loadVector(location_lightPositionEyeSpace[i], glm::vec3(0, 0, 0));
            loadVector(location_lightColour[i], glm::vec3(0, 0, 0));
            loadVector(location_attenuation[i], glm::vec3(1, 0, 0));
        }
    }
}

void NormalMappingShader::loadViewMatrix(const glm::mat4& viewMatrix) {
    loadMatrix(location_viewMatrix, viewMatrix);
}

void NormalMappingShader::loadProjectionMatrix(const glm::mat4& projection) {
    loadMatrix(location_projectionMatrix, projection);
}

glm::vec3 NormalMappingShader::getEyeSpacePosition(const Light& light, const glm::mat4& viewMatrix) {
    glm::vec3 position = light.getPosition();
    glm::vec4 eyeSpacePos = glm::vec4(position.x, position.y, position.z, 1.0f);
    eyeSpacePos = viewMatrix * eyeSpacePos;
    return glm::vec3(eyeSpacePos);
}

void NormalMappingShader::loadUseSpecularMap(bool useMap){
    loadBoolean(location_useSpecularMap, useMap);
}