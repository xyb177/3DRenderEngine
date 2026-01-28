#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <random>

#include "DisplayManager.h"
#include "Loader.h"
#include "RawModel.h"
#include "EntityRenderer.h"
#include "StaticShader.h"
#include "TerrainRenderer.h"
#include "Camera.h"
#include "OBJLoader.h"
#include "Light.h"
#include "MasterRenderer.h"
#include "Player.h"
#include "GuiRenderer.h"
#include "MousePicker.h"
#include "WaterRenderer.h"
#include "WaterFrameBuffers.h"
#include "NormalMappingRenderer.h"
#include "OBJFileLoader.h"
#include "NormalMappedObjLoader.h"
#include "TextMaster.h"
#include "ParticleMaster.h"
#include "ParticleSystem.h"
#include "ParticleTexture.h"
#include "PostProcessing.h"
double random_double(){
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

int main() {
    // 获取单例实例
    DisplayManager& display = DisplayManager::getInstance();
    
    if (!display.CreateDisplay()) {
        return -1;
    }

    Loader loader;
    
    TextMaster& tm = TextMaster::getInstance(loader);

    TexturedModel stanfordBunny(OBJFileLoader::loadOBJ("res/person.obj", loader), ModelTexture(loader.loadTexture("res/playerTexture.png")));
    Player player(stanfordBunny,glm::vec3(75,5,-75),0,0,0,0.5);
    Camera camera(player);

    StaticShader shader;
    TerrainShader Tshader;
    MasterRenderer renderer(shader,Tshader,loader,camera);

    FontType font(loader.loadTexture("res/Fonts/Candara/candara.png"),"res/Fonts/Candara/candara.fnt");
    GUIText text("This is a  test text!",3,&font, glm::vec2(0,0.4f),1.0f,true);
    text.setColour(0.1f,0.1f,0.1f);
    
    //*********TERRAIN TEXTURE STUFF**********
    TerrainTexture backgroundTexture(loader.loadTexture("res/grassy2.png"));
    TerrainTexture rTexture(loader.loadTexture("res/mud.png"));
    TerrainTexture gTexture(loader.loadTexture("res/grassFlowers.png"));
    TerrainTexture bTexture(loader.loadTexture("res/path.png"));

    TerrainTexturePack texturePack(backgroundTexture,rTexture,gTexture,bTexture);
    TerrainTexture blendMap(loader.loadTexture("res/blendMap.png"));
    
    // *****************************************
    TexturedModel rocks(OBJFileLoader::loadOBJ("res/rocks.obj", loader), ModelTexture(loader.loadTexture("res/rocks.png")));
    
    ModelTexture fernTextureAtlas(loader.loadTexture("res/fern1.png"));
    fernTextureAtlas.setNumberOfRows(2);
    TexturedModel fern(OBJFileLoader::loadOBJ("res/fern.obj", loader), fernTextureAtlas);
    fern.getTexture().setHasTransparency(true);

    TexturedModel bobble(OBJFileLoader::loadOBJ("res/pine.obj", loader), ModelTexture(loader.loadTexture("res/pine.png")));
    bobble.getTexture().setHasTransparency(true);

    std::vector<Terrain> terrains;
    // Terrain terrain(0,-1,loader,texturePack,blendMap,"res/heightmap.png");
    Terrain terrain(0,-1,loader,texturePack,blendMap);
    terrains.push_back(terrain);

    TexturedModel lamp(OBJFileLoader::loadOBJ("res/lamp.obj", loader), ModelTexture(loader.loadTexture("res/lamp.png")));
    lamp.getTexture().setUseFakeLighting(true);

    TexturedModel cherryModel(OBJFileLoader::loadOBJ("res/SpecularMap/cherry.obj", loader),
				 ModelTexture(loader.loadTexture("res/SpecularMap/cherry.png")));
    cherryModel.getTexture().setHasTransparency(true);            
	cherryModel.getTexture().setShineDamper(10);
	cherryModel.getTexture().setReflectivity(0.5f);
	cherryModel.getTexture().setSpecularMap(loader.loadTexture("res/SpecularMap/cherryS.png"));


    TexturedModel lanternModel(OBJFileLoader::loadOBJ("res/SpecularMap/lantern.obj", loader),
				 ModelTexture(loader.loadTexture("res/SpecularMap/lantern.png")));
    lanternModel.getTexture().setSpecularMap(loader.loadTexture("res/SpecularMap/lanternS.png"));
     
    std::vector<std::string> TEXTURE_FILES = {
        "res/Skybox/right.png",
        "res/Skybox/left.png",
        "res/Skybox/top.png",
        "res/Skybox/bottom.png",
        "res/Skybox/front.png",
        "res/Skybox/back.png"
    };
    TexturedModel teaModel(OBJFileLoader::loadOBJ("res/tea.obj", loader),
                 ModelTexture(loader.loadTexture("res/tea.png")));
    teaModel.getTexture().setEnvironmentMap(loader.loadCubeMap(TEXTURE_FILES));
    teaModel.getTexture().setReflectionStrength(0.9);
    
    std::vector<Entity*> entities;
    std::vector<Entity*> normalMapEntities;
    std::cout << "Before rendering reflection3" << std::endl;
    //******************NORMAL MAP MODELS************************
    TexturedModel barrelModel(NormalMappedObjLoader::loadOBJ("res/barrel.obj", loader),
				 ModelTexture(loader.loadTexture("res/barrel.png")));
	
    barrelModel.getTexture().setNormalMap(loader.loadTexture("res/barrelNormal.png"));
	barrelModel.getTexture().setShineDamper(10);
	barrelModel.getTexture().setReflectivity(0.5f);
	barrelModel.getTexture().setSpecularMap(loader.loadTexture("res/SpecularMap/barrelS.png"));

	TexturedModel crateModel(NormalMappedObjLoader::loadOBJ("res/crate.obj", loader),
				 ModelTexture(loader.loadTexture("res/crate.png")));
	crateModel.getTexture().setNormalMap(loader.loadTexture("res/crateNormal.png"));
	crateModel.getTexture().setShineDamper(10);
	crateModel.getTexture().setReflectivity(0.5f);
		
	TexturedModel boulderModel(NormalMappedObjLoader::loadOBJ("res/boulder.obj", loader),
				 ModelTexture(loader.loadTexture("res/boulder.png")));
	boulderModel.getTexture().setNormalMap(loader.loadTexture("res/boulderNormal.png"));
	boulderModel.getTexture().setShineDamper(10);
	boulderModel.getTexture().setReflectivity(0.5f);


    
    //************ENTITIES*******************
		
    Entity* entity = new Entity(barrelModel, glm::vec3(75, terrain.getHeightOfTerrain(75,-75), -75), 0, 0, 0, 1.0f);
	Entity* entity2 = new Entity(boulderModel, glm::vec3(85, terrain.getHeightOfTerrain(85,-75), -75), 0, 0, 0, 1.0f);
	Entity* entity3 = new Entity(crateModel, glm::vec3(65, terrain.getHeightOfTerrain(65,-75), -75), 0, 0, 0, 0.04f);
    
	normalMapEntities.push_back(entity);
	normalMapEntities.push_back(entity2);
	normalMapEntities.push_back(entity3);

    Entity* entity4 = new Entity(cherryModel, glm::vec3(95, terrain.getHeightOfTerrain(95,-75), -75), 0, 0, 0, 1.0f);
    Entity* entity5 = new Entity(lanternModel, glm::vec3(105, terrain.getHeightOfTerrain(105,-75), -75), 0, 0, 0, 1.0f);
    Entity* entity6 = new Entity(teaModel,glm::vec3(115, terrain.getHeightOfTerrain(115,-75)+20, -75), 0, 0, 0, 0.5f);
    entities.push_back(entity4);
    entities.push_back(entity5);
	entities.push_back(entity6);	
	
	for (int i = 0; i < 60; i++) {
		if (i % 3 == 0) {
			float x = random_double() * 150;
			float z = random_double()* -150;
				if ((x > 50 && x < 100) || (z < -50 && z > -100)) {
				} else {
					float y = terrain.getHeightOfTerrain(x, z);

					entities.push_back(new Entity(fern, 3, glm::vec3(x, y, z), 0,
							random_double()  * 360, 0, 0.9f));
				}
			}
			if (i % 2 == 0) {

				float x = random_double() * 150;
				float z = random_double() * -150;
				if ((x > 50 && x < 100) || (z < -50 && z > -100)) {

				} else {
					float y = terrain.getHeightOfTerrain(x, z);
					entities.push_back(new Entity(bobble, 1, glm::vec3(x, y, z), 0,
							random_double()  * 360, 0, random_double()  * 0.6f + 0.8f));
				}
			}
		}
		entities.push_back(new Entity(rocks, glm::vec3(75, terrain.getHeightOfTerrain(0,-150), -75), 0, 0, 0, 75));
    entities.push_back(&player);
    //*******************OTHER SETUP***************  
    
    std::vector<Light> lights;
    Light sun(glm::vec3(1000000, 1500000, -1000000),glm::vec3(1.0f, 1.0f, 1.0f));
    lights.push_back(sun);
   
    
    
    std::vector<GuiTexture> guis;
    
    
        
    GuiRenderer guiRenderer(loader);
    MousePicker picker(camera, renderer.getProjectionMatrix(), terrain);
    
    //**********Water Renderer Set-up************************
    WaterFrameBuffers fbos;
    WaterShader waterShader;
    WaterRenderer waterRenderer = WaterRenderer(loader,waterShader,renderer.getProjectionMatrix(),fbos);
    std::vector<WaterTile> waters;
    waters.push_back(WaterTile(75,-75,0));
    std::cout << waters[0].getHeight();
    

    // **********Particles Renderer Set-up************************   
    ParticleMaster& pm = ParticleMaster::getInstance(loader,renderer.getProjectionMatrix());
    
    ParticleTexture particleTexture(loader.loadTexture("res/particle/cosmic.png"),4);
    ParticleSystem system(particleTexture,40,10,0.1f,1,1.6f);
    system.randomizeRotation();
    
    system.setLifeError(0.1f);
    system.setSpeedError(0.25f);
    system.setScaleError(0.5f);

    Fbo fbo(display.getWidth(),display.getHeight());
    Fbo outputfbo(display.getWidth(),display.getHeight(),Fbo::DEPTH_TEXTURE);
    PostProcessing::getInstance().init(loader);
    //****************Game Loop Below*********************
    
    while (!display.ShouldClose()) {
        player.move(terrain);
        camera.move();
        picker.update();
        
        system.generateParticles(glm::vec3(5.0f, 50.0f,5.0f));
        pm.update(camera);
        
        entity->increaseRotation(0, 1, 0);
		entity2->increaseRotation(0, 1, 0);
		entity3->increaseRotation(0, 1, 0);

        renderer.renderShadowMap(entities,sun);
        glEnable(GL_CLIP_DISTANCE0);
       
        fbos.bindReflectionFrameBuffer();
        float distance =2 *(camera.getPosition().y - waters[0].getHeight());
        
        camera.setPosition(camera.getPosition().x,camera.getPosition().y-distance,camera.getPosition().z);
       
        camera.invertPitch();
        renderer.renderScene(entities,normalMapEntities,terrains,lights,camera,glm::vec4(0,1,0,-waters[0].getHeight()+10));
        camera.setPosition(camera.getPosition().x,camera.getPosition().y+distance,camera.getPosition().z);
        camera.invertPitch();
        
        fbos.bindRefractionFrameBuffer();
        renderer.renderScene(entities,normalMapEntities,terrains,lights,camera,glm::vec4(0,-1,0,waters[0].getHeight()));

        fbos.unbindCurrentFrameBuffer();
        glDisable(GL_CLIP_DISTANCE0);
        //renderer.processEntity(player);
        fbo.bindFrameBuffer();
        renderer.renderScene(entities, normalMapEntities,terrains,lights,camera,glm::vec4(0,-1,0,100000));
        waterRenderer.render(waters,camera,sun);
        pm.renderParticles(camera);
        fbo.unbindFrameBuffer();
        fbo.resolveToFbo(GL_COLOR_ATTACHMENT0,outputfbo);
        PostProcessing::getInstance().doPostProcessing(outputfbo.getColourTexture());
        guiRenderer.render(guis);
       
        tm.render();
        display.UpdateDisplay();
    }

    for (auto* entity : entities) {
    delete entity;  // 防止内存泄漏
    }
    for( auto* entity: normalMapEntities){
        delete entity; 
    }
    PostProcessing::getInstance().cleanUp();
    outputfbo.cleanUp();
    fbo.cleanUp();
    pm.cleanUp();
    tm.cleanUp();
    fbos.cleanUp();
    entities.clear();
    guiRenderer.cleanUp();
    renderer.cleanUp();
    loader.cleanUp();
    display.CloseDisplay();

    return 0;
}
