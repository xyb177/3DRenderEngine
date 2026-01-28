#include "ParticleRenderer.h"
#include <cstring>

std::vector<float> ParticleRenderer::VERTICES  = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f};
ParticleRenderer::ParticleRenderer(Loader &loader, const glm::mat4 &projectionMatrix)
                    :loader(loader),quad(loader.loadToVAO(VERTICES,2)),vboId(loader.createEmptyVbo(INSTANCE_DATA_LENGTH * MAX_INSTANCES))
{
    loader.addInstancedAttribute(quad.getVaoID(), vboId, 1,4, INSTANCE_DATA_LENGTH,0);
    loader.addInstancedAttribute(quad.getVaoID(), vboId, 2,4, INSTANCE_DATA_LENGTH,4);
    loader.addInstancedAttribute(quad.getVaoID(), vboId, 3,4, INSTANCE_DATA_LENGTH,8);
    loader.addInstancedAttribute(quad.getVaoID(), vboId, 4,4, INSTANCE_DATA_LENGTH,12);
    loader.addInstancedAttribute(quad.getVaoID(), vboId, 5,4, INSTANCE_DATA_LENGTH,16);
    loader.addInstancedAttribute(quad.getVaoID(), vboId, 6,1, INSTANCE_DATA_LENGTH,20);
    shader.start();
    shader.loadProjectionMatrix(projectionMatrix);
    shader.stop();
}

void ParticleRenderer::render( std::unordered_map<ParticleTexture, std::vector<Particle>>& particles, Camera& camera)
{
    glm::mat4 viewMatrix = Maths::createViewMatrix(camera);
    prepare();
    for (auto& pair : particles) {
        ParticleTexture texture = pair.first;
        std::vector<Particle>& particleList = pair.second;
        bindTexture(texture);
        pointer = 0;
        std::vector<float> vboData(particleList.size() * INSTANCE_DATA_LENGTH);
        for (auto& particle : particleList) {
            updateModelViewMatrix(particle.getPosition(), particle.getRotation(), 
                                particle.getScale(), viewMatrix, vboData);
            updateTexCoordInfo(particle, vboData);
        }
        loader.updateVbo(vboId, vboData);
        glDrawArraysInstanced(GL_TRIANGLE_STRIP,0,quad.getVertexCount(),particleList.size());
    }
    finishRendering();
}

void ParticleRenderer::cleanUp(){
    shader.cleanUp();
}

void ParticleRenderer::prepare(){
    shader.start();
    glBindVertexArray(quad.getVaoID());
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);
    glEnableVertexAttribArray(6);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    glEnable(GL_DEPTH_TEST);
}


void ParticleRenderer::updateModelViewMatrix(glm::vec3 position, float rotation, float scale, glm::mat4 viewMatrix,std::vector<float>& vboData)
{
    glm::mat4 modelMatrix = glm::mat4(1.0f);;
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix[0][0] = viewMatrix[0][0];
    modelMatrix[0][1] = viewMatrix[1][0];
    modelMatrix[0][2] = viewMatrix[2][0];
    modelMatrix[1][0] = viewMatrix[0][1];
    modelMatrix[1][1] = viewMatrix[1][1];
    modelMatrix[1][2] = viewMatrix[2][1];
    modelMatrix[2][0] = viewMatrix[0][2];
    modelMatrix[2][1] = viewMatrix[1][2];
    modelMatrix[2][2] = viewMatrix[2][2];
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(scale, scale, scale));
    glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;
    storeMatrixData(modelViewMatrix, vboData);
}
void ParticleRenderer::updateTexCoordInfo(Particle &particle, std::vector<float> &data)
{
    data[pointer++] = particle.getTexOffset1().x;
    data[pointer++] = particle.getTexOffset1().y;
    data[pointer++] = particle.getTexOffset2().x;
    data[pointer++] = particle.getTexOffset2().y;
    data[pointer++] = particle.getBlend();
}
void ParticleRenderer::finishRendering()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);
    glDisableVertexAttribArray(5);
    glDisableVertexAttribArray(6);
    glBindVertexArray(0);
    shader.stop();
}

void ParticleRenderer::bindTexture(ParticleTexture &texture)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,texture.getTextureID());
    shader.loadNumberOfRows(texture.getNumberOfRows());
}

void ParticleRenderer::storeMatrixData(glm::mat4 matrix, std::vector<float> &vboData)
{
    std::memcpy(vboData.data() + pointer, glm::value_ptr(matrix), 16 * sizeof(float));
    pointer += 16;
}
