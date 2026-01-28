#include "CombineFilter.h"

CombineFilter::CombineFilter()
{
    shader.start();
	shader.connectTextureUnits();
	shader.stop();
}

void CombineFilter::render(int colourTexture, int highlightTexture)
{
    shader.start();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,colourTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,highlightTexture);
    renderer.renderQuad();
    shader.stop();
}


void CombineFilter::cleanUp(){
	renderer.cleanUp();
	shader.cleanUp();
}