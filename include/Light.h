#pragma once

#include "glm.hpp"

class Light {
private:
    glm::vec3 position;
    glm::vec3 colour;
    glm::vec3 attenuation{1.0f, 0.0f, 0.0f};

public:
    Light(const glm::vec3& position, const glm::vec3& colour);
    Light(const glm::vec3& position, const glm::vec3& colour, const glm::vec3& attenuation);
    
    // Getter methods
    const glm::vec3& getPosition() const;
    const glm::vec3& getColour() const;
    const glm::vec3& getAttenuation() const;

    // Setter methods
    void setPosition(const glm::vec3& position);
    void setColour(const glm::vec3& colour);
    void setAttenuation(const glm::vec3& attenuation);
};
