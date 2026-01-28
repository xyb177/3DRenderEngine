#pragma once

#include <map>
#include <vector>
#include "glm.hpp"
#include "NormalMappingShader.h"
#include "Entity.h"
#include "Light.h"
#include "Camera.h"
#include "TexturedModel.h"

class NormalMappingRenderer {
public:
    NormalMappingRenderer(const glm::mat4& projectionMatrix);
    void render(const std::unordered_map<TexturedModel, std::vector<Entity>>& entities, 
                const glm::vec4& clipPlane, const std::vector<Light>& lights, 
                const Camera& camera);
    void cleanUp();

private:
    NormalMappingShader shader;

    void prepare(const glm::vec4& clipPlane, const std::vector<Light>& lights, const Camera& camera);
    void prepareTexturedModel(const TexturedModel& model);
    void unbindTexturedModel();
    void prepareInstance(const Entity& entity);

};
