#pragma once

#include "CombineShader.h"
#include "ImageRenderer.h"


class CombineFilter{
public:
    CombineFilter();

    void render(int colourTexture, int highlightTexture);

    void cleanUp();
private:
    ImageRenderer renderer;
    CombineShader shader;
};