#include "CombineShader.h"

const std::string CombineShader::VERTEX_FILE = "src/glsl/simpleVertex.glsl";
const std::string CombineShader::FRAGMENT_FILE = "src/glsl/combineFragment.glsl"; 
CombineShader::CombineShader()
                :ShaderProgram(VERTEX_FILE, FRAGMENT_FILE)
{
    initUniforms();
}

void CombineShader::getAllUniformLocations()
{
    location_colourTexture = getUniformLocation("colourTexture");
	location_highlightTexture = getUniformLocation("highlightTexture");
}


void CombineShader::connectTextureUnits()
{
    loadInt(location_colourTexture,0);
    loadInt(location_highlightTexture,1);
}