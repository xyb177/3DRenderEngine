#pragma once 
#include "glm.hpp"
#include "ShaderProgram.h"

class ContrastShader : public ShaderProgram{
public:
    ContrastShader();


protected:
    void getAllUniformLocations() override;
private:

    static const std::string VERTEX_FILE;
    static const std::string FRAGMENT_FILE;
};