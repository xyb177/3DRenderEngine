#pragma once

#include <unordered_map>
#include <vector>
#include <memory>  // 用于智能指针
#include "StaticShader.h"
#include "TexturedModel.h"
#include "TerrainShader.h"
#include "Entity.h"
#include "Light.h"
#include "Camera.h"
#include "EntityRenderer.h"
#include "TerrainRenderer.h"
#include "SkyboxRenderer.h"
#include "NormalMappingRenderer.h"
#include "ShadowMapMasterRenderer.h"
class MasterRenderer {
public:

    MasterRenderer(StaticShader& shader, TerrainShader& terrainShader,Loader& loader,Camera& camera);
    void render(const std::vector<Light>& light, const Camera& camera, glm::vec4 clipPlane);
    void renderScene(std::vector<Entity*> entities, std::vector<Entity*> normalEntities,std::vector<Terrain>& terrains, std::vector<Light>& lights,Camera& camera,glm::vec4 clipPlane);
    void cleanUp();
    void processTerrain(const Terrain& terrain);
    void processEntity(const Entity& entity);
    void processNormalMapEntity(const Entity& entity);
    void prepare();
    static void enableCulling();
    static void disableCulling();

    glm::mat4 getProjectionMatrix();
    int getShadowMapTexture();
    void renderShadowMap(std::vector<Entity*>& entityList, Light& sun);

    static constexpr float RED = 0.5f;
    static constexpr float GREEN = 0.5f;
    static constexpr float BLUE = 0.5f;

    friend class ShadowBox;
private:
    glm::mat4& createProjectionMatrix();

    static constexpr float FOV = 70.0f;
    static constexpr float NEAR_PLANE = 0.1f;
    static constexpr float FAR_PLANE = 1000.0f;

    glm::mat4 projectionMatrix;
    StaticShader shader;
    EntityRenderer renderer;
    TerrainShader terrainShader;
    TerrainRenderer terrainRenderer;
    SkyBoxRenderer skyboxRenderer;
    NormalMappingRenderer normalMapRenderer;
    ShadowMapMasterRenderer shadowMapRenderer;

    std::unordered_map<TexturedModel, std::vector<Entity>> entities;
    std::unordered_map<TexturedModel, std::vector<Entity>> normalMapEntities;
    std::vector<Terrain> terrains;
};
