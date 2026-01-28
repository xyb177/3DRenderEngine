#include "Entity.h"

Entity::Entity(TexturedModel model, const glm::vec3 &position, float rotX, float rotY, float rotZ, float scale)
        :model(std::move(model)), position(position), rotX(rotX), rotY(rotY), rotZ(rotZ), scale(scale) {}

Entity::Entity(TexturedModel model, int index ,const glm::vec3 &position, float rotX, float rotY, float rotZ, float scale)
        :model(std::move(model)),textureIndex(index),position(position), rotX(rotX), rotY(rotY), rotZ(rotZ), scale(scale) {}


void Entity::increasePosition(float dx, float dy, float dz) {
    position.x += dx;
    position.y += dy;
    position.z += dz;
}

void Entity::increaseRotation(float dx, float dy, float dz) {
    rotX += dx;
    rotY += dy;
    rotZ += dz;
}

// Getter methods
const TexturedModel& Entity::getModel() const { return model; }
const glm::vec3& Entity::getPosition() const { return position; }
float Entity::getRotX() const { return rotX; }
float Entity::getRotY() const { return rotY; }
float Entity::getRotZ() const { return rotZ; }
float Entity::getScale() const { return scale; }

float Entity::getTextureXOffset() const
{
    int col = textureIndex% model.getTexture().getNumberOfRows();
    return col* 1.0 /model.getTexture().getNumberOfRows();
}

float Entity::getTextureYOffset() const
{
    int row = textureIndex/model.getTexture().getNumberOfRows();
    return row* 1.0 /model.getTexture().getNumberOfRows();
}
    // Setter methods
void Entity::setModel(const TexturedModel& newModel) { model = newModel; }
void Entity::setPosition(const glm::vec3& newPosition) { position = newPosition; }
void Entity::setRotX(float newRotX) { rotX = newRotX; }
void Entity::setRotY(float newRotY) { rotY = newRotY; }
void Entity::setRotZ(float newRotZ) { rotZ = newRotZ; }
void Entity::setScale(float newScale) { scale = newScale; }