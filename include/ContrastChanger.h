#pragma once 
#include "ImageRenderer.h"
#include "ContrastShader.h"


class ContrastChanger{
public:
    ContrastChanger();

    void render(int texture);
    void cleanUp();
private:
    ImageRenderer renderer;
    ContrastShader shader;
};