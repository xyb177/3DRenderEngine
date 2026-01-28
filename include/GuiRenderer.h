#pragma once
#include "Loader.h"
#include "GuiTexture.h"
#include "GuiShader.h"
#include "Maths.h"
class GuiRenderer{

public:
    GuiRenderer(Loader& loader);
    void render(std::vector<GuiTexture> Guis);
    void cleanUp();

private:
    RawModel quad;
    GuiShader shader;
};

