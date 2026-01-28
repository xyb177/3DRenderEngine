#pragma once
#include "glm.hpp"
#include "ParticleTexture.h"
#include "Camera.h"
class ParticleMaster;
class Particle{

public:
    Particle(ParticleTexture texture, glm::vec3 position, glm::vec3 velocity, float gravityEffect, float lifeLength, float rotation,float scale);

    glm::vec3 getPosition() const {return position;}
    float getRotation() const {return rotation;}
    float getScale() const {return scale;}
    ParticleTexture getTexture()  {return texture;}
    glm::vec2 getTexOffset1() const {return texOffset1;}
    glm::vec2 getTexOffset2() const {return texOffset2;}
    float getBlend() const {return blend;}
    float getDistance() const {return distance;}

    bool update(Camera& camera);
private:
    void updateTextureCoordInfo();
    void setTextureOffset(glm::vec2& offset,int index);
private:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec2 texOffset1;
    glm::vec2 texOffset2;

    float gravityEffect;
    float lifeLength;
    float rotation;
    float scale;
    float blend;

    float elapsedTime = 0;
    float GRAVITY = -50;
    float distance;

    ParticleTexture texture;
};