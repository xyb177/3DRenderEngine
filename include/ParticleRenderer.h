#pragma once

#include "glm.hpp"
#include <vector>
#include "Camera.h"
#include "Loader.h"
#include "RawModel.h"
#include "ModelTexture.h"
#include "Maths.h"
#include "ParticleShader.h"



class ParticleRenderer{
public:
    ParticleRenderer(Loader& loader, const glm::mat4& projectionMatrix);
    void render(std::unordered_map<ParticleTexture, std::vector<Particle>>& particles, Camera& camera); 
    void cleanUp();
private:
    void prepare();
    void updateModelViewMatrix(glm::vec3 position, float rotation, float scale, glm::mat4 viewMatrix,std::vector<float>& vboData);
    void updateTexCoordInfo(Particle& particle,std::vector<float>& data);
    void finishRendering();
    void bindTexture(ParticleTexture& texture);
    void storeMatrixData(glm::mat4 matrix, std::vector<float>& vboData);
private:

    static std::vector<float> VERTICES; 
    static constexpr int MAX_INSTANCES = 10000;
    static constexpr int INSTANCE_DATA_LENGTH = 21;
    RawModel quad;
    ParticleShader shader;
    Loader loader;
    int vboId ;
    int pointer = 0;
};