#pragma once
#include "glm.hpp"
#include <string.h>
#include "ShaderProgram.h"
#include "Particle.h"

class ParticleShader : public ShaderProgram
{
public:
    ParticleShader();
    
    void loadProjectionMatrix(const glm::mat4& projection);
    void loadNumberOfRows(float numberOfRows);
   
protected:
    
    void getAllUniformLocations() override;

private:
    int location_numberOfRows;
    int location_projectionMatrix; 
    
    static const std::string VERTEX_FILE;
    static const std::string FRAGMENT_FILE;  

};