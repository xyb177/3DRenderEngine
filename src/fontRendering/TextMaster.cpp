#pragma once 
#include "TextMaster.h"
#include "FontType.h"       
#include "GUIText.h"

TextMaster* TextMaster::instance = nullptr;

TextMaster::TextMaster(Loader& loader):loader(loader)
{   
   
}

TextMaster& TextMaster::getInstance(Loader& loader) {
    if (instance == nullptr) {
        instance = new TextMaster(loader);
    }
    return *instance;
}

TextMaster& TextMaster::getInstance() {
    if (instance == nullptr) {
        throw std::runtime_error("TextMaster not initialized. Call getInstance(Loader&) first.");
    }
    return *instance;
}
void TextMaster::loadText(GUIText& text)
{
    FontType* font = text.getFont();
    TextMeshData data = font->loadText(text);
    int vao = loader.loadToVAO(data.getVertexPositions(),data.getTextureCoords());
    text.setMeshInfo(vao, data.getVertexCount());

    auto [iter, inserted] = texts.try_emplace(font, std::vector<GUIText*>());
    iter->second.push_back(&text);
}

void TextMaster::removeText(GUIText& text)
{
    auto font = text.getFont();
    auto it = texts.find(font);

    if (it != texts.end()){
        auto& textBatch = it -> second;

        for (auto textIt = textBatch.begin(); textIt != textBatch.end(); ) {
            if (*textIt == &text) {
                textIt = textBatch.erase(textIt);
            } else {
                ++textIt;
            }
        }
        if (textBatch.empty()) {
            texts.erase(it);
        }
    }
}

void TextMaster::render()
{
    renderer.render(texts);
}

void TextMaster::cleanUp()
{
    renderer.cleanUp();
    texts.clear();
}
