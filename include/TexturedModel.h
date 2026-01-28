#pragma once
#include "RawModel.h"
#include "ModelTexture.h"
#include <functional>
class TexturedModel {
public:
    TexturedModel(const RawModel& model, const ModelTexture& texture);
    const RawModel& getRawModel() const;
    ModelTexture& getTexture();
    const ModelTexture& getTexture() const;
    bool operator == (const TexturedModel& other) const;
private:
    RawModel rawModel;
    ModelTexture texture;
};    

namespace std{
    template<>
    struct hash<TexturedModel>
    {
        size_t operator()(const TexturedModel& tm) const;
    };
}