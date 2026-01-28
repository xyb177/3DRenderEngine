#pragma once
#include "ShaderProgram.h"

class CombineShader : public ShaderProgram{

public:
    CombineShader();

    void connectTextureUnits();
protected:
    void getAllUniformLocations() override;

private:
    static const std::string VERTEX_FILE;
    static const std::string FRAGMENT_FILE;
    
    int location_colourTexture;
    int location_highlightTexture;
};