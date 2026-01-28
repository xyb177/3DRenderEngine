#include "VerticalBlur.h"


VerticalBlur::VerticalBlur(int targetFboWidth, int targetFboHeight) 
                :renderer(targetFboWidth,targetFboHeight)
{
    shader.start();
    shader.loadTargetHeight(static_cast<float>(targetFboHeight));
    shader.stop();

}

void VerticalBlur::render(unsigned int texture) {
   
    shader.start();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    renderer.renderQuad();
    shader.stop();
   
    glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int VerticalBlur::getOutputTexture() const {
    return renderer.getOutputTexture();
}

void VerticalBlur::cleanUp() {
    
    renderer.cleanUp();
    shader.cleanUp();
}