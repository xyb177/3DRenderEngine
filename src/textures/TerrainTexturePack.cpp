#include "TerrainTexturePack.h"

TerrainTexturePack::TerrainTexturePack(const TerrainTexture &backgroundTexture, const TerrainTexture &rTexture, const TerrainTexture &gTexture, const TerrainTexture &bTexture)
    :backgroundTexture(backgroundTexture),rTexture(rTexture),gTexture(gTexture),bTexture(bTexture)
{
}

const TerrainTexture &TerrainTexturePack::getBackGroundTexture()
{
    return backgroundTexture;
}

const TerrainTexture &TerrainTexturePack::getrTexture()
{
    return rTexture;
}

const TerrainTexture &TerrainTexturePack::getgTexture()
{
    return gTexture;
}

const TerrainTexture &TerrainTexturePack::getbTexture()
{
    return bTexture;
}
