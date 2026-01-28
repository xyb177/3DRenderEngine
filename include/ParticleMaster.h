#pragma once
#include <unordered_map>
#include "ParticleRenderer.h"

class ParticleMaster{
public:
    static ParticleMaster& getInstance(Loader& loader,glm::mat4 projectionMatrix);
    static ParticleMaster& getInstance();

    void update(Camera& camera);
    void renderParticles(Camera& camera);
    void cleanUp();
    void addParticle(Particle particle);
    
private:
    ParticleMaster(Loader& loader,glm::mat4 projectionMatrix);    
private:
    static ParticleMaster* instance;
    std::unordered_map<ParticleTexture, std::vector<Particle>> particles ;
    ParticleRenderer renderer;
};