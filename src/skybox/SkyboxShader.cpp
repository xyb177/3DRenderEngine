#include "SkyboxShader.h"

const std::string SkyboxShader::VERTEX_FILE = "src/glsl/skyboxVertexShader.glsl";
const std::string SkyboxShader::FRAGMENT_FILE = "src/glsl/skyboxFragmentShader.glsl";

SkyboxShader::SkyboxShader(): ShaderProgram(VERTEX_FILE, FRAGMENT_FILE) { initUniforms(); }

void SkyboxShader::getAllUniformLocations()
{
    location_projectionMatrix = getUniformLocation("projectionMatrix");
    location_viewMatrix = getUniformLocation("viewMatrix");
    location_fogColour = getUniformLocation("fogColour");
    location_cubeMap = getUniformLocation("cubeMap");
    location_cubeMap2 = getUniformLocation("cubeMap2");
    location_blendFactor = getUniformLocation("blendfactor");
}


void SkyboxShader::loadViewMatrix(const Camera& camera) {
    glm::mat4 viewMatrix = Maths::createViewMatrix(camera);
    viewMatrix[3][0] = 0.0f; // Tx = 0
    viewMatrix[3][1] = 0.0f; // Ty = 0
    viewMatrix[3][2] = 0.0f; // Tz = 0
    rotation += ROTATE_SPEED * DisplayManager::getInstance().getFrameTimeSeconds();
    viewMatrix = glm::rotate(viewMatrix, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f)); 
    loadMatrix(location_viewMatrix, viewMatrix);
}

void SkyboxShader::loadProjectionMatrix(const glm::mat4& projection) {
    loadMatrix(location_projectionMatrix, projection);
}

void SkyboxShader::loadFogColour(float r, float g, float b)
{
    loadVector(location_fogColour,glm::vec3(r,g,b));
}

void SkyboxShader::loadBlendFactor(float blend)
{
    loadFloat(location_blendFactor,blend);
}


void SkyboxShader::connectTextureUnits(){
    loadInt(location_cubeMap,0);
    loadInt(location_cubeMap2,1);
}