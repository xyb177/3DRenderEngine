#include "Light.h"

Light::Light(const glm::vec3& position, const glm::vec3& colour) 
    : position(position), colour(colour) {}

Light::Light(const glm::vec3 &position, const glm::vec3 &colour, const glm::vec3 &attenuation)
    :position(position), colour(colour),attenuation(attenuation)
{
}

// Getter implementations
const glm::vec3& Light::getPosition() const {
    return position;
}

const glm::vec3& Light::getColour() const {
    return colour;
}

const glm::vec3 &Light::getAttenuation() const
{
   return attenuation;
}

// Setter implementations
void Light::setPosition(const glm::vec3& position) {
    this->position = position;
}

void Light::setColour(const glm::vec3& colour) {
    this->colour = colour;
}

void Light::setAttenuation(const glm::vec3 &attenuation)
{
    this->attenuation = attenuation;
}

