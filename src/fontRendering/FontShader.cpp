#include "FontShader.h"


FontShader::FontShader()
            : ShaderProgram(getVertexFile(), getFragmentFile())
{   
   
    initUniforms();
    
}

void FontShader::loadColour(glm::vec3 colour)
{
    loadVector(location_colour,colour);
}

void FontShader::loadTranslation(glm::vec2 translation)
{
    load2DVector(location_translation, translation);
}
void FontShader::getAllUniformLocations()
{
    location_colour = getUniformLocation("colour");
    location_translation = getUniformLocation("translation");
}



const std::string& FontShader::getVertexFile() {
    static const std::string path = "src/glsl/fontVertex.glsl";
    return path;
}

const std::string& FontShader::getFragmentFile() {
    static const std::string path = "src/glsl/fontFragment.glsl";
    return path;
}