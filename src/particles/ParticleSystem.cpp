#include "ParticleSystem.h"
#include <cmath>


// 构造函数
ParticleSystem::ParticleSystem(ParticleTexture texture, float pps, float speed, float gravityComplient, 
                               float lifeLength, float scale)
    :texture(texture), pps(pps), averageSpeed(speed), gravityComplient(gravityComplient),
      averageLifeLength(lifeLength), averageScale(scale),
      randomEngine(std::random_device{}()),
      randomDist(0.0f, 1.0f) {
       
}

void ParticleSystem::setDirection(const glm::vec3& direction, float deviation) {
    this->direction = direction;
    this->directionDeviation = deviation * glm::pi<float>();
}

void ParticleSystem::randomizeRotation() {
    randomRotation = true;
}

void ParticleSystem::setSpeedError(float error) {
    this->speedError = error * averageSpeed;
}

void ParticleSystem::setLifeError(float error) {
    this->lifeError = error * averageLifeLength;
}

void ParticleSystem::setScaleError(float error) {
    this->scaleError = error * averageScale;
}

void ParticleSystem::generateParticles(const glm::vec3& systemCenter) {
    float deltaTime = DisplayManager::getInstance().getFrameTimeSeconds();
    float particlesToCreate = pps * deltaTime;
    int count = static_cast<int>(std::floor(particlesToCreate));
    float partialParticle = std::fmod(particlesToCreate, 1.0f);
    
    for (int i = 0; i < count; i++) {
        emitParticle(systemCenter);
    }
    
    if (randomDist(randomEngine) < partialParticle) {
        emitParticle(systemCenter);
    }
}

void ParticleSystem::emitParticle(const glm::vec3& center) {
    glm::vec3 velocity;
    
    if (glm::length(direction) > 0.0f) {
        velocity = generateRandomUnitVectorWithinCone(direction, directionDeviation);
    } else {
        velocity = generateRandomUnitVector();
    }
    
    velocity = glm::normalize(velocity);
    velocity *= generateValue(averageSpeed, speedError);
    float scale = generateValue(averageScale, scaleError);
    float lifeLength = generateValue(averageLifeLength, lifeError);
    
    Particle temp(texture, glm::vec3(center),velocity,gravityComplient, lifeLength, generateRotation(), scale);
}

float ParticleSystem::generateValue(float average, float errorMargin) {
    float offset = (randomDist(randomEngine) - 0.5f) * 2.0f * errorMargin;
    return average + offset;
}

float ParticleSystem::generateRotation() {
    if (randomRotation) {
        return randomDist(randomEngine) * 360.0f;
    } else {
        return 0.0f;
    }
}

glm::vec3 ParticleSystem::generateRandomUnitVectorWithinCone(const glm::vec3& coneDirection, float angle) {
    float cosAngle = std::cos(angle);
    float theta = randomDist(randomEngine) * 2.0f * glm::pi<float>();
    float z = cosAngle + (randomDist(randomEngine) * (1.0f - cosAngle));
    float rootOneMinusZSquared = std::sqrt(1.0f - z * z);
    float x = rootOneMinusZSquared * std::cos(theta);
    float y = rootOneMinusZSquared * std::sin(theta);
    
    glm::vec4 direction(x, y, z, 1.0f);
    
    // 如果锥形方向不是默认的(0,0,1)或(0,0,-1)，需要进行旋转
    if (coneDirection.x != 0 || coneDirection.y != 0 || 
        (coneDirection.z != 1.0f && coneDirection.z != -1.0f)) {
        glm::vec3 defaultDir(0.0f, 0.0f, 1.0f);
        glm::vec3 rotateAxis = glm::cross(coneDirection, defaultDir);
        rotateAxis = glm::normalize(rotateAxis);
        float rotateAngle = std::acos(glm::dot(coneDirection, defaultDir));
        
        // 创建旋转矩阵并应用
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), -rotateAngle, rotateAxis);
        direction = rotationMatrix * direction;
    } else if (coneDirection.z == -1.0f) {
        direction.z *= -1.0f;
    }
    
    return glm::vec3(direction);
}

glm::vec3 ParticleSystem::generateRandomUnitVector() {
    float theta = randomDist(randomEngine) * 2.0f * glm::pi<float>();
    float z = (randomDist(randomEngine) * 2.0f) - 1.0f;
    float rootOneMinusZSquared = std::sqrt(1.0f - z * z);
    float x = rootOneMinusZSquared * std::cos(theta);
    float y = rootOneMinusZSquared * std::sin(theta);
    return glm::vec3(x, y, z);
}
