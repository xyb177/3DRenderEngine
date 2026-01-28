#pragma once
#include <string>
#include "glm.hpp"

class TextMaster;
class FontType;
class GUIText {
public:
    /**
     * Creates a new text, loads the text's quads into a VAO
     * 
     * @param text - the text content
     * @param fontSize - font size where 1 is default size
     * @param font - the font to use
     * @param position - position on screen (0,0 top left, 1,1 bottom right)
     * @param maxLineLength - virtual page width (1 = full screen width)
     * @param centered - whether text should be centered
     */
    GUIText(const std::string& text, 
           float fontSize, 
           FontType* font,
           const glm::vec2& position,
           float maxLineLength,
           bool centered);

    // Remove the text from the screen
    void remove();

    // Getters
    FontType* getFont() { return font; }
    const FontType* getFont() const noexcept { return font; }
    const glm::vec3& getColour() const noexcept { return colour; }
    const int getNumberOfLines() const noexcept { return numberOfLines; }
    const glm::vec2& getPosition() const noexcept { return position; }
    const unsigned int getMesh() const noexcept { return textMeshVao; }
    const int getVertexCount() const noexcept { return vertexCount; }
    const float getMaxLineSize() const noexcept { return lineMaxSize; }
    const std::string& getTextString() const noexcept { return textString; }
    const bool isCentered() const noexcept { return centerText; }

    // Setters
    void setColour(float r, float g, float b);
    void setMeshInfo(unsigned int vao, int verticesCount);
    void setNumberOfLines(int number); 

    const float getFontSize() const noexcept { return fontSize; }

private:
    std::string textString;
    float fontSize;
    unsigned int textMeshVao = 0;
    int vertexCount = 0;
    glm::vec3 colour{0.0f, 0.0f, 0.0f};
    glm::vec2 position;
    float lineMaxSize;
    int numberOfLines = 0;
    FontType* font;
    bool centerText = false;
};
