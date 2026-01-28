#pragma once

class WaterTile {
public:
    static constexpr float TILE_SIZE = 60.0f;

    WaterTile(float centerX, float centerZ, float height);
    
    float getHeight() const;
    float getX() const;
    float getZ() const;

private:
    float height;
    float x, z;
};
