#pragma once
#include "ImageRenderer.h"
#include "BrightFilterShader.h"

class BrightFilter{

public:
    BrightFilter(int width, int height);

    void render(int texture);

    int getOutputTexture();

    void cleanUp();
private:
    ImageRenderer renderer;
    BrightFilterShader shader;
};