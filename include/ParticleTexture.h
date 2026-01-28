#pragma once
#include <string>
#include <memory>
class ParticleTexture{
public:
    ParticleTexture(int textureID, int numberOfRows);

    int getTextureID() const { return textureID;}
    int getNumberOfRows() const { return numberOfRows;}

    bool operator==(const ParticleTexture& other) const {
        return this->getTextureID() == other.getTextureID();
    }
private:
    int textureID;
    int numberOfRows;

};


namespace std {
    template<>
    struct hash<ParticleTexture> {
        size_t operator()(const ParticleTexture& texture) const {
            
            return texture.getTextureID();
        }
    };
}
