#pragma once 
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <vector>
#include <memory>  
#include "TexturedModel.h"
#include "Entity.h"
#include "StaticShader.h"
#include "Maths.h"
#include "DisplayManager.h"

class EntityRenderer{

public:
   
    EntityRenderer(StaticShader& shader,const glm::mat4 &projectionMatrix);
   
    void render(const Entity& entity, StaticShader& shader);

    void render(std::unordered_map<TexturedModel,std::vector<Entity>>& entities);
    
private:
    void prepareTexturedModel(const TexturedModel& model);
    void unbindTexturedModel();
    void prepareInstance(const Entity& entity);


private:
   
    StaticShader shader;
};