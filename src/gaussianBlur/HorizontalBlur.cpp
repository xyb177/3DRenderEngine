#include "HorizontalBlur.h"


HorizontalBlur::HorizontalBlur(int targetFboWidth, int targetFboHeight) 
                :renderer(targetFboWidth,targetFboHeight)
{
    shader.start();
    shader.loadTargetWidth(static_cast<float>(targetFboWidth));
    shader.stop();

}

void HorizontalBlur::render(unsigned int texture) {
   
    shader.start();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    renderer.renderQuad();
    shader.stop();
   
    glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int HorizontalBlur::getOutputTexture() const {
    return renderer.getOutputTexture();
}

void HorizontalBlur::cleanUp() {
    
    renderer.cleanUp();
    shader.cleanUp();
}