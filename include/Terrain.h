#pragma once 
#include "RawModel.h"
#include "Loader.h"
#include "ModelTexture.h"
#include "TerrainTexturePack.h"
#include <stb_image.h>
#include <algorithm>  
#include <cmath>
#include "glm.hpp"
#include "Maths.h"
#include "HeightsGenerator.h"
class Terrain
{
public:
    Terrain( int gridX,  int gridZ,  Loader& loader, const TerrainTexturePack& texturePack, 
            const TerrainTexture& blendMap, const std::string& heightMap);
    
    Terrain(int gridX, int gridZ, Loader &loader, const TerrainTexturePack &texturePack, 
            const TerrainTexture &blendMap);

    float getX() const { return x; }
    float getZ() const { return z; }
    float getSize() const {return SIZE;}
    const RawModel& getModel() const { return model; }
    const TerrainTexture& getBlendMap() const {return blendMap;}
    const TerrainTexturePack& getTexturePack() const {return texturePack;}
    float getHeightOfTerrain(float worldX, float worldZ);

private:
    static constexpr float SIZE = 150.0f;
    static constexpr float MAX_HEIGHT  = 40;
    static constexpr float MAX_PIXEL_COLOUR = 256.0f;
    
    float m_minHeight, m_maxHeight;   // 预计算的极值
    std::vector<std::vector<float>> m_heightData;  // 预计算的高度数据

    float x;
    float z;
    RawModel model;
    TerrainTexture blendMap;
    TerrainTexturePack texturePack;

    RawModel generateTerrain(Loader& loader ,std::string heightMap);
    RawModel generateProceduralTerrain(Loader &loader, int gridX, int gridZ);
    RawModel generateMeshFromHeightData(Loader &loader, int width, int height);
    
    void loadHeightMap(const std::vector<unsigned char>& imageData, int width, int height, int channels);
    void generateProceduralHeightData(int size, HeightsGenerator& generator);

    float getHeight(int x, int z);
    glm::vec3 calculateNormal(int x, int z);
    
    // float getHeight(int x, int z, int width, int height);
    // float getHeight(int x ,int z , HeightsGenerator& generator);
    // glm::vec3 calculateNormal(int x ,int z, int width, int height);
    // glm::vec3 calculateNormal(int x ,int z, HeightsGenerator& generator);

};