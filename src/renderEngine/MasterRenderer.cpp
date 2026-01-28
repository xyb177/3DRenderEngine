#include "MasterRenderer.h"

MasterRenderer::MasterRenderer(StaticShader& shader, TerrainShader& terrainShader, Loader& loader, Camera& camera)
    : renderer(shader, createProjectionMatrix()),      
      terrainRenderer(terrainShader, createProjectionMatrix()),
      skyboxRenderer(loader,createProjectionMatrix()),
      normalMapRenderer(createProjectionMatrix()),
      shadowMapRenderer(camera)
{   
    enableCulling();
}


void MasterRenderer::render(const std::vector<Light>& light, const Camera &camera,glm::vec4 clipPlane)
{   
    prepare();
    shader.start();
    shader.loadClipPlane(clipPlane);
    shader.loadSkyColour(RED,GREEN,BLUE);
    shader.loadProjectionMatrix(projectionMatrix); 
    shader.loadLight(light);
    shader.loadViewMatrix(camera);
    shader.connectTextureUnits();
    renderer.render(entities);
    shader.stop();
    
    normalMapRenderer.render(normalMapEntities,clipPlane,light,camera);
    
    terrainShader.start();
    terrainShader.loadClipPlane(clipPlane);
    terrainShader.loadSkyColour(RED,GREEN,BLUE);
    terrainShader.loadProjectionMatrix(projectionMatrix); 
    terrainShader.loadLight(light);
    terrainShader.loadViewMatrix(camera);
    terrainRenderer.render(terrains,shadowMapRenderer.getToShadowMapSpaceMatrix());
    terrainShader.stop();

   
    skyboxRenderer.render(camera,RED,GREEN,BLUE);
    terrains.clear();
    entities.clear();
    normalMapEntities.clear();
}


void MasterRenderer::renderScene(std::vector<Entity*> entities,std::vector<Entity*> normalEntities ,std::vector<Terrain>& terrains, 
                                std::vector<Light>& lights,Camera& camera,glm::vec4 clipPlane)
{
    for(auto& terrain : terrains)
        processTerrain(terrain);
    for(auto* entity : entities)
        processEntity(*entity);

    for(auto* normalentity : normalEntities)
         processNormalMapEntity(*normalentity);
    render(lights, camera,clipPlane);
}
void MasterRenderer::cleanUp()
{
    shader.cleanUp();
    terrainShader.cleanUp();
    normalMapRenderer.cleanUp();
    shadowMapRenderer.cleanUp();
}

void MasterRenderer::processTerrain(const Terrain& terrain) {
    terrains.push_back(terrain);
}

void MasterRenderer::processEntity(const Entity& entity) {
    const TexturedModel& entityModel = entity.getModel();
    auto it = entities.find(entityModel);
    
    if (it != entities.end()) {
        // 找到现有批次，添加实体
        it->second.push_back(entity);
    } else {
        // 创建新批次
        entities.emplace(entityModel, std::vector<Entity>{entity});
    }
}

void MasterRenderer::processNormalMapEntity(const Entity& entity) {
    const TexturedModel& entityModel = entity.getModel();
    auto it = normalMapEntities.find(entityModel);
    
    if (it != entities.end()) {
        // 找到现有批次，添加实体
        it->second.push_back(entity);
    } else {
        // 创建新批次
        normalMapEntities.emplace(entityModel, std::vector<Entity>{entity});
    }
}

void MasterRenderer::prepare()
{   
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(RED, GREEN, BLUE, 1.0f);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, getShadowMapTexture());
}

void MasterRenderer::enableCulling()
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void MasterRenderer::disableCulling()
{
    glDisable(GL_CULL_FACE);
}

glm::mat4 MasterRenderer::getProjectionMatrix()
{
    return projectionMatrix;
}

int MasterRenderer::getShadowMapTexture()
{
    return shadowMapRenderer.getShadowMap();
}

void MasterRenderer::renderShadowMap(std::vector<Entity*> &entityList, Light &sun)
{
    for(auto& entity : entityList)
    {
        processEntity(*entity);
    }
    shadowMapRenderer.render(entities, sun);
    entities.clear();
}

glm::mat4& MasterRenderer::createProjectionMatrix()
{
    float aspectRatio = (float)DisplayManager::getInstance().getWidth() / 
                       (float)DisplayManager::getInstance().getHeight();
    projectionMatrix = glm::perspective(
        glm::radians(FOV), 
        aspectRatio, 
        NEAR_PLANE, 
        FAR_PLANE
    );

    return projectionMatrix;
}