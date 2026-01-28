#pragma once
#include "glm.hpp"
#include "TexturedModel.h"

class Entity {
public:
    Entity(TexturedModel model, 
           const glm::vec3& position, 
           float rotX, float rotY, float rotZ, 
           float scale);
    
    Entity(TexturedModel model, int index,
           const glm::vec3& position, 
           float rotX, float rotY, float rotZ, 
           float scale);
    
    void increasePosition(float dx, float dy, float dz);
    void increaseRotation(float dx, float dy, float dz);
    
    // Getter methods
    const TexturedModel& getModel() const; 
    const glm::vec3& getPosition() const;
    float getRotX() const;
    float getRotY() const; 
    float getRotZ() const;
    float getScale() const; 
    float getTextureXOffset() const;
    float getTextureYOffset() const;

    // Setter methods
    void setModel(const TexturedModel& newModel); 
    void setPosition(const glm::vec3& newPosition); 
    void setRotX(float newRotX);
    void setRotY(float newRotY);
    void setRotZ(float newRotZ); 
    void setScale(float newScale);

private:
    TexturedModel model;
    glm::vec3 position;
    float rotX, rotY, rotZ;
    float scale;
    int textureIndex = 0;
};
