#pragma once

#include "glm.hpp"
#include <string.h>
#include "ShaderProgram.h"

class GuiShader :public ShaderProgram{
public:
    GuiShader();
   
    void loadTransformationMatrix(const glm::mat4& matrix);
    
private:
    void getAllUniformLocations();

private:
    int  location_transformationMatrix;

    static const std::string VERTEX_FILE;
    static const std::string FRAGMENT_FILE;
};