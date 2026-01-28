#include "Particle.h"
#include "DisplayManager.h"
#include "ParticleMaster.h"

Particle::Particle(ParticleTexture texture,glm::vec3 position, glm::vec3 velocity, float gravityEffect, float lifeLength, float rotation, float scale)
       :texture(texture), position(position),velocity(velocity),gravityEffect(gravityEffect),lifeLength(lifeLength),rotation(rotation),scale(scale)
{
    ParticleMaster::getInstance().addParticle(*this);
}

bool Particle::update(Camera& camera)
{   
    float delta = static_cast<float>(DisplayManager::getInstance().getFrameTimeSeconds());
    velocity.y += GRAVITY * gravityEffect * delta;
    glm::vec3 change = velocity * delta;
    position += change;
    distance =  glm::distance(camera.getPosition(), position);
    updateTextureCoordInfo();
    elapsedTime += delta;
    return elapsedTime < lifeLength;
}

void Particle::updateTextureCoordInfo()
{
    float lifeFactor = elapsedTime / lifeLength;
    int stageCount = texture.getNumberOfRows() * texture.getNumberOfRows();
    float atlasProgression = lifeFactor * stageCount;
    int index1 = static_cast<int>(std::floor(atlasProgression));
    int index2 = index1 < stageCount - 1 ? index1 +1 : index1;
    blend = std::fmod(atlasProgression,1.0);
    setTextureOffset(texOffset1,index1);
    setTextureOffset(texOffset2,index2);
}

void Particle::setTextureOffset(glm::vec2& offset, int index)
{
    int numberOfRows = texture.getNumberOfRows();
    int col = index % numberOfRows;
    int row = index / numberOfRows;
    
    float textureSize = 1.0f / numberOfRows;
    
    // 计算纹理块的左下角坐标
    offset.x = static_cast<float>(col) * textureSize;
    offset.y = static_cast<float>(row) * textureSize;
}

