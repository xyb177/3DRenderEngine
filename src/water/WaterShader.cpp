#include "WaterShader.h"
const std::string WaterShader::VERTEX_FILE = "src/glsl/waterVertex.glsl";
const std::string WaterShader::FRAGMENT_FILE = "src/glsl/waterFragment.glsl";

WaterShader::WaterShader()
             : ShaderProgram(VERTEX_FILE, FRAGMENT_FILE) {initUniforms();}


void WaterShader::getAllUniformLocations() {
    location_projectionMatrix = getUniformLocation("projectionMatrix");
    location_viewMatrix = getUniformLocation("viewMatrix");
    location_modelMatrix = getUniformLocation("modelMatrix");
    location_reflectionTexture = getUniformLocation("reflectionTexture");
    location_refractionTexture = getUniformLocation("refractionTexture");
    location_dudvMap = getUniformLocation("dudvMap");
    location_moveFactor = getUniformLocation("moveFactor");
    location_cameraPosition = getUniformLocation("cameraPosition");
    location_normalMap = getUniformLocation("normalMap");
    location_lightColour = getUniformLocation("lightColour");
    location_lightPosition = getUniformLocation("lightPosition");
    location_depthMap = getUniformLocation("depthMap");
}



void WaterShader::loadProjectionMatrix(const glm::mat4& projection) {
    loadMatrix(location_projectionMatrix, projection);
}

void WaterShader::loadViewMatrix(const Camera& camera) {
    glm::mat4 viewMatrix = Maths::createViewMatrix(camera);
    loadMatrix(location_viewMatrix, viewMatrix);
    loadVector(location_cameraPosition,camera.getPosition());
}

void WaterShader::loadModelMatrix(const glm::mat4& modelMatrix) {
    loadMatrix(location_modelMatrix, modelMatrix);
}

void WaterShader::connectTextureUnits(){
    loadInt(location_reflectionTexture,0);
    loadInt(location_refractionTexture,1);
    loadInt(location_dudvMap,2);
    loadInt(location_normalMap,3);
    loadInt(location_depthMap,4);
}

void WaterShader::loadMoveFactor(float movefactor)
{
    loadFloat(location_moveFactor,movefactor);
}

void WaterShader::loadLight(const Light &sun)
{
    loadVector(location_lightColour, sun.getColour());
    loadVector(location_lightPosition,sun.getPosition());
}
