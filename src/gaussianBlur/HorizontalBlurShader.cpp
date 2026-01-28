#include "HorizontalBlurShader.h"
const std::string HorizontalBlurShader::VERTEX_FILE = "src/glsl/horizontalBlurVertex.glsl";
const std::string HorizontalBlurShader::FRAGMENT_FILE = "src/glsl/blurFragment.glsl";

HorizontalBlurShader::HorizontalBlurShader()
                            :ShaderProgram(VERTEX_FILE,FRAGMENT_FILE)
{
    initUniforms();
}

void HorizontalBlurShader::loadTargetWidth(float width)
{
    loadFloat(location_targetWidth, width);
}

void HorizontalBlurShader::getAllUniformLocations()
{
    location_targetWidth = getUniformLocation("targetWidth");
}
