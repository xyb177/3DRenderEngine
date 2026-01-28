#include "ParticleShader.h"

const std::string ParticleShader::VERTEX_FILE = "src/glsl/particleVShader.glsl";
const std::string ParticleShader::FRAGMENT_FILE = "src/glsl/particleFShader.glsl";

ParticleShader::ParticleShader() 
                : ShaderProgram(VERTEX_FILE, FRAGMENT_FILE) { initUniforms(); }


void ParticleShader::getAllUniformLocations() {
     location_numberOfRows = getUniformLocation("numberOfRows");
     location_projectionMatrix = getUniformLocation("projectionMatrix");
  
}


void ParticleShader::loadProjectionMatrix(const glm::mat4& projection){
   loadMatrix(location_projectionMatrix, projection);
}

void ParticleShader::loadNumberOfRows(float numberOfRows)
{
    loadFloat(location_numberOfRows, numberOfRows);
}
