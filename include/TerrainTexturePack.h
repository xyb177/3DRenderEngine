#pragma once
#include "TerrainTexture.h"

class TerrainTexturePack{

public:
    TerrainTexturePack(const TerrainTexture& backgroundTexture, 
                        const TerrainTexture& rTexture,
                        const TerrainTexture& gTexture,
                        const TerrainTexture& bTexture);

    const TerrainTexture& getBackGroundTexture();
    const TerrainTexture& getrTexture();
    const TerrainTexture& getgTexture();
    const TerrainTexture& getbTexture();
private:
    TerrainTexture backgroundTexture;
    TerrainTexture rTexture;
    TerrainTexture gTexture;
    TerrainTexture bTexture;
};