#pragma once 
#include "glm.hpp"

class GuiTexture{

public:
    GuiTexture(int texture, glm::vec2 position, glm::vec2 scale);
    int getTexture() const;
    glm::vec2 getPosition() const;
    glm::vec2 getScale() const;

private:
    int texture;
    glm::vec2 position;
    glm::vec2 scale;
};