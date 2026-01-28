#pragma once

class TerrainTexture
{
public:
    TerrainTexture(int textureID);
    const int getTextureID() const;
private:
    int textureID;
};