#pragma once 
#include "FontType.h"
#include "GUIText.h"
#include "TextMeshCreator.h"

void TextMeshCreatorDeleter::operator()(TextMeshCreator* ptr) {
    delete ptr;
}

FontType::FontType(int textureAtlas, const std::string& fontFile)
    : textureAtlas(textureAtlas),
      loader(std::unique_ptr<TextMeshCreator, TextMeshCreatorDeleter>(
          new TextMeshCreator(fontFile))) {}

TextMeshData FontType::loadText(GUIText& text) {
    return loader->createTextMesh(text);
}

bool FontType::operator==(const FontType &other) const
{
    return  textureAtlas == other.textureAtlas; 
}
