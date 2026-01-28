#pragma once 
#include "glm.hpp"
#include "ShaderProgram.h"

class HorizontalBlurShader :public ShaderProgram
{
public:
    HorizontalBlurShader();

    void loadTargetWidth(float width);
protected:
    void getAllUniformLocations() override;
private:
    static const std::string VERTEX_FILE;
    static const std::string FRAGMENT_FILE;
    int location_targetWidth;
};