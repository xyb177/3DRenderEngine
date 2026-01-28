#pragma once
#include "ImageRenderer.h"
#include "VerticalBlurShader.h"




class VerticalBlur {
public:
    
    VerticalBlur(int targetFboWidth, int targetFboHeight);
    
    ~VerticalBlur() = default;
    
    void render(unsigned int texture);

    unsigned int getOutputTexture() const;
    
    void cleanUp();

private:
    ImageRenderer renderer; 
    VerticalBlurShader shader; 
};
