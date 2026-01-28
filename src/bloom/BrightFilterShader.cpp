#include "BrightFilterShader.h"

const std::string BrightFilterShader::VERTEX_FILE = "src/glsl/simpleVertex.glsl";
const std::string BrightFilterShader::FRAGMENT_FILE = "src/glsl/brightFilterFragment.glsl"; 
BrightFilterShader::BrightFilterShader()
                :ShaderProgram(VERTEX_FILE, FRAGMENT_FILE)
{
    initUniforms();
}

void BrightFilterShader::getAllUniformLocations()
{
}
