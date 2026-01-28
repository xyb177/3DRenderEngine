#include "ShadowShader.h"
#include <iostream>

const std::string ShadowShader::VERTEX_FILE = "src/glsl/shadowVertexShader.glsl";
const std::string ShadowShader::FRAGMENT_FILE = "src/glsl/shadowFragmentShader.glsl";

ShadowShader::ShadowShader()
    : ShaderProgram(VERTEX_FILE, FRAGMENT_FILE)
{
    initUniforms();
}

void ShadowShader::getAllUniformLocations() {
    
    location_mvpMatrix = getUniformLocation("mvpMatrix");
    
    if (location_mvpMatrix == -1) {
        std::cerr << "Warning: Could not find uniform location for mvpMatrix" << std::endl;
    }
}

void ShadowShader::loadMvpMatrix(const glm::mat4& mvpMatrix) {
    loadMatrix(location_mvpMatrix, mvpMatrix);
}

