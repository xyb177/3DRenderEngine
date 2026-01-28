#pragma once
#include <vector>
#include <unordered_map>
#include "Loader.h"
#include "FontRenderer.h"
class FontType;
class GUIText;
class TextMaster{
public:
    static TextMaster& getInstance(Loader& loader);
    static TextMaster& getInstance();

    void loadText(GUIText& text);
    void removeText(GUIText& text);
    void render();
    void cleanUp();
private:
    TextMaster(Loader& loader);
private:
    static TextMaster* instance;
    Loader& loader;
    std::unordered_map<FontType*, std::vector<GUIText*>> texts;
    FontRenderer renderer;
};