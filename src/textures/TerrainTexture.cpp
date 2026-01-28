#include "TerrainTexture.h"

TerrainTexture::TerrainTexture(int textureID)
    :textureID(textureID)
{
}

const int TerrainTexture::getTextureID() const
{
    return textureID;
}
