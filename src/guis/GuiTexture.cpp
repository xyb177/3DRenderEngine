#include "GuiTexture.h"

GuiTexture::GuiTexture(int texture, glm::vec2 position, glm::vec2 scale)
                :texture(texture),position(position),scale(scale)
{

}

int GuiTexture::getTexture() const
{
    return texture;
}

glm::vec2 GuiTexture::getPosition() const
{
    return position;
}

glm::vec2 GuiTexture::getScale() const
{
    return scale;
}
