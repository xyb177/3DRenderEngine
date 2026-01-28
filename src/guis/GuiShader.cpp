#include "GuiShader.h"
#include <fstream>
#include <sstream>
#include <iostream>

const std::string GuiShader::VERTEX_FILE = "src/glsl/guiVertexShader.glsl";
const std::string GuiShader::FRAGMENT_FILE = "src/glsl/guiFragmentShader.glsl";

GuiShader::GuiShader() 
            : ShaderProgram(VERTEX_FILE, FRAGMENT_FILE) { initUniforms(); }

void GuiShader::loadTransformationMatrix(const glm::mat4& matrix) {
    loadMatrix(location_transformationMatrix, matrix);
}


void GuiShader::getAllUniformLocations() {
    location_transformationMatrix = getUniformLocation("transformationMatrix");
}

