#include "BrightFilter.h"

BrightFilter::BrightFilter(int width, int height)
                : renderer(width, height)
{

}

void BrightFilter::render(int texture){
    shader.start();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    renderer.renderQuad();
	shader.stop();
}

int BrightFilter::getOutputTexture(){
	return renderer.getOutputTexture();
}

void BrightFilter::cleanUp(){
    renderer.cleanUp();
	shader.cleanUp();
}