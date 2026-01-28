#include "ContrastChanger.h"

ContrastChanger::ContrastChanger()
{

}

void ContrastChanger::render(int texture)
{
    shader.start();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,texture);
    renderer.renderQuad();
    shader.stop();
}


void ContrastChanger::cleanUp()
{
    shader.cleanUp();
    renderer.cleanUp();
}