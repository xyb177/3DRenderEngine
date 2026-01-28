#pragma once
#include <string>
#include <memory>
#include "TextMeshData.h"

class GUIText;
class TextMeshCreator;

struct TextMeshCreatorDeleter {
    void operator()(TextMeshCreator* ptr);
};

class FontType {
public:
    /**
     * Creates a new font and loads up the data about each character from the
     * font file.
     * 
     * @param textureAtlas - the ID of the font atlas texture
     * @param fontFile - the font file containing information about each character
     */
    FontType(int textureAtlas, const std::string& fontFile);
    
    /**
     * @return The font texture atlas ID
     */
    int getTextureAtlas() const noexcept { return textureAtlas; }

    /**
     * Takes in an unloaded text and calculates all vertices for the quads
     * on which this text will be rendered.
     * 
     * @param text - the unloaded text
     * @return Information about the vertices of all the quads
     */
    TextMeshData loadText(GUIText& text);
    bool operator == (const FontType& other) const;

private:
    int textureAtlas;
    std::unique_ptr<TextMeshCreator, TextMeshCreatorDeleter> loader;
};


namespace std
{
    template<>
    struct hash<FontType>
    {
        size_t  operator()(const FontType& ft) const
        {
            return ft.getTextureAtlas();
        }
    };
}