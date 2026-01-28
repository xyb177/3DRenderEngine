#include "ContrastShader.h"
const std::string ContrastShader::VERTEX_FILE = "src/glsl/contrastVertex.glsl";
const std::string ContrastShader::FRAGMENT_FILE = "src/glsl/contrastFragment.glsl";

ContrastShader::ContrastShader()
: ShaderProgram(VERTEX_FILE, FRAGMENT_FILE)
{
     initUniforms();
}

void ContrastShader::getAllUniformLocations()
{
}
