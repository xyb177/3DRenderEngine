#pragma once
#include "ShaderProgram.h"

class BrightFilterShader : public ShaderProgram{

public:
    BrightFilterShader();

protected:
    void getAllUniformLocations() override;

private:
    static const std::string VERTEX_FILE;
    static const std::string FRAGMENT_FILE;
    
};