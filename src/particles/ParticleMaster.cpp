#include "ParticleMaster.h"


ParticleMaster* ParticleMaster::instance = nullptr;
ParticleMaster::ParticleMaster(Loader &loader, glm::mat4 projectionMatrix)
                :renderer(loader,projectionMatrix)
{

}

ParticleMaster& ParticleMaster::getInstance(Loader& loader, glm::mat4 projectionMatrix) {
    if (instance == nullptr) {
        instance = new ParticleMaster(loader, projectionMatrix);
    }
    return *instance;
}

ParticleMaster& ParticleMaster::getInstance() {
    if (instance == nullptr) {
        throw std::runtime_error("ParticleMaster not initialized. Call getInstance(Loader&) first.");
    }
    return *instance;
}

void ParticleMaster::update(Camera& camera)
{   
    for (auto it = particles.begin(); it != particles.end(); ) {
        auto& vec = it->second;
        for (auto iter = vec.begin(); iter != vec.end(); ) {
            bool stillAlive = iter->update(camera);
            if (!stillAlive) {
                iter = vec.erase(iter);
            } else {
                ++iter;
            }
        }
    
        if (vec.empty()) {
            it = particles.erase(it);
        } else {
            std::sort(vec.begin(), vec.end(),
                [](const Particle& a, const Particle& b) {
                    return a.getDistance() > b.getDistance(); 
                });
            ++it;
        }
        
    }
}



void ParticleMaster::renderParticles(Camera &camera)
{
    renderer.render(particles, camera);
}


void ParticleMaster::cleanUp()
{
    renderer.cleanUp();
}

void ParticleMaster::addParticle(Particle particle)
{   
    auto [iter, inserted] = particles.try_emplace(particle.getTexture(), std::vector<Particle>());
    iter->second.push_back(particle);
    
}


