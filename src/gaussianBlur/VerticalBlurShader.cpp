#include "VerticalBlurShader.h"
const std::string VerticalBlurShader::VERTEX_FILE = "src/glsl/verticalBlurVertex.glsl";
const std::string VerticalBlurShader::FRAGMENT_FILE = "src/glsl/blurFragment.glsl";

VerticalBlurShader::VerticalBlurShader()
                            :ShaderProgram(VERTEX_FILE,FRAGMENT_FILE)
{
    initUniforms();
}

void VerticalBlurShader::loadTargetHeight(float height)
{
    loadFloat(location_targetHeight, height);
}

void VerticalBlurShader::getAllUniformLocations()
{
    location_targetHeight = getUniformLocation("targetHeight");
}
