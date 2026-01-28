#include "TexturedModel.h"

TexturedModel::TexturedModel(const RawModel &model, const ModelTexture &texture): rawModel(model), texture(texture) {}


const RawModel &TexturedModel::getRawModel() const
{
     return rawModel;
}

ModelTexture &TexturedModel::getTexture()
{
    return texture;
}

const ModelTexture &TexturedModel::getTexture() const
{
     return texture;
}

bool TexturedModel::operator==(const TexturedModel &other) const
{
    return rawModel.getVaoID() == other.rawModel.getVaoID() &&
           texture.getID() == other.texture.getID();
}

size_t std::hash<TexturedModel>::operator()(const TexturedModel& tm) const {
    size_t h1 = std::hash<unsigned int>{}(tm.getRawModel().getVaoID());
    size_t h2 = std::hash<GLuint>{}(tm.getTexture().getID());
    return h1 ^ (h2 << 1);
}