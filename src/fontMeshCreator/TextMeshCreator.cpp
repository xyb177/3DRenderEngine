#include "TextMeshCreator.h"
#include <algorithm>

TextMeshCreator::TextMeshCreator(const std::string &metaFile)
                    : metaData(std::make_unique<MetaFile>(metaFile)) {}

TextMeshData TextMeshCreator::createTextMesh(GUIText &text)
{
    auto lines = createStructure(text);
    return createQuadVertices(text,lines);
}

std::vector<Line> TextMeshCreator::createStructure(GUIText &text)
{
    const std::string& str = text.getTextString();
    std::vector<Line> lines;
    Line currentLine(metaData->getSpaceWidth(), text.getFontSize(), text.getMaxLineSize());
    Word currentWord(text.getFontSize());

    for (char c : str) {
        int ascii = static_cast<int>(c);
        if (ascii == SPACE_ASCII) {
            bool added = currentLine.attemptToAddWord(currentWord);
            if (!added) {
                lines.push_back(currentLine);
                currentLine = Line(metaData->getSpaceWidth(), text.getFontSize(), text.getMaxLineSize());
                currentLine.attemptToAddWord(currentWord);
            }
            currentWord = Word(text.getFontSize());
            continue;
        }
        const Character* character = metaData->getCharacter(ascii);
        if (character) {
            currentWord.addCharacter(*character);
        }
    }
    completeStructure(lines, currentLine, currentWord, text);
    return lines;
}

void TextMeshCreator::completeStructure(std::vector<Line> &lines, Line &currentLine, Word &currentWord,  GUIText &text)
{
    bool added = currentLine.attemptToAddWord(currentWord);
    if (!added) {
        lines.push_back(currentLine);
        currentLine = Line(metaData->getSpaceWidth(), text.getFontSize(), text.getMaxLineSize());
        currentLine.attemptToAddWord(currentWord);
    }
    lines.push_back(currentLine);
}

TextMeshData TextMeshCreator::createQuadVertices(GUIText &text, const std::vector<Line> &lines)
{
    text.setNumberOfLines(lines.size());
    double cursorX = 0.0;
    double cursorY = 0.0;
    std::vector<float> vertices;
    std::vector<float> textureCoords;
    vertices.reserve(lines.size() * 6 * 4); // Reserve space for performance
    textureCoords.reserve(lines.size() * 6 * 4);

    for (const auto& line : lines) {
        if (text.isCentered()) {
            cursorX = (line.getMaxLength() - line.getLineLength()) / 2;
        }
        
        for (const auto& word : line.getWords()) {
            for (const auto& letter : word.getCharacters()) {
                addVerticesForCharacter(cursorX, cursorY, letter, text.getFontSize(), vertices);
                addTexCoords(textureCoords, letter.getxTextureCoord(), letter.getyTextureCoord(),
                            letter.getXMaxTextureCoord(), letter.getYMaxTextureCoord());
                cursorX += letter.getxAdvance() * text.getFontSize();
            }
            cursorX += metaData->getSpaceWidth() * text.getFontSize();
        }
        cursorX = 0.0;
        cursorY += LINE_HEIGHT * text.getFontSize();
    }
    
    return TextMeshData(std::move(vertices), std::move(textureCoords));
}

void TextMeshCreator::addVerticesForCharacter(double cursorX, double cursorY, const Character &character, double fontSize, std::vector<float> &vertices)
{
    double x = cursorX + (character.getxOffset() * fontSize);
    double y = cursorY + (character.getyOffset() * fontSize);
    double maxX = x + (character.getSizeX() * fontSize);
    double maxY = y + (character.getSizeY() * fontSize);
    double properX = (2 * x) - 1;
    double properY = (-2 * y) + 1;
    double properMaxX = (2 * maxX) - 1;
    double properMaxY = (-2 * maxY) + 1;
    addVertices(vertices, properX, properY, properMaxX, properMaxY);
}

void TextMeshCreator::addVertices(std::vector<float> &vertices, double x, double y, double maxX, double maxY)
{
     vertices.insert(vertices.end(), {
        static_cast<float>(x), static_cast<float>(y),
        static_cast<float>(x), static_cast<float>(maxY),
        static_cast<float>(maxX), static_cast<float>(maxY),
        static_cast<float>(maxX), static_cast<float>(maxY),
        static_cast<float>(maxX), static_cast<float>(y),
        static_cast<float>(x), static_cast<float>(y)
    });
}

void TextMeshCreator::addTexCoords(std::vector<float> &texCoords, double x, double y, double maxX, double maxY)
{
    texCoords.insert(texCoords.end(), {
        static_cast<float>(x), static_cast<float>(y),
        static_cast<float>(x), static_cast<float>(maxY),
        static_cast<float>(maxX), static_cast<float>(maxY),
        static_cast<float>(maxX), static_cast<float>(maxY),
        static_cast<float>(maxX), static_cast<float>(y),
        static_cast<float>(x), static_cast<float>(y)
    });
}
