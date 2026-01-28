#pragma once 
#include "glm.hpp"
#include "ShaderProgram.h"

class VerticalBlurShader :public ShaderProgram
{
public:
    VerticalBlurShader();

    void loadTargetHeight(float height);
protected:
    void getAllUniformLocations() override;
private:
    static const std::string VERTEX_FILE;
    static const std::string FRAGMENT_FILE;
    int location_targetHeight;
};