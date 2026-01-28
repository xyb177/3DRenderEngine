#pragma once

#include <vector>
#include <string>
#include <memory>
#include "Line.h"
#include "MetaFile.h"
#include "GUIText.h"
#include "TextMeshData.h"


class TextMeshCreator {
public:
    static constexpr double LINE_HEIGHT = 0.03f;
    static constexpr int SPACE_ASCII = 32;

    TextMeshCreator(const std::string& metaFile);
    TextMeshData createTextMesh(GUIText& text);

private:
    std::unique_ptr<MetaFile> metaData;

    std::vector<Line> createStructure(GUIText& text);
    void completeStructure(std::vector<Line>& lines, Line& currentLine, Word& currentWord, GUIText& text);
    TextMeshData createQuadVertices(GUIText& text, const std::vector<Line>& lines);
    
    void addVerticesForCharacter(double cursorX, double cursorY, const Character& character, 
                               double fontSize, std::vector<float>& vertices);
    static void addVertices(std::vector<float>& vertices, double x, double y, double maxX, double maxY);
    static void addTexCoords(std::vector<float>& texCoords, double x, double y, double maxX, double maxY);
    
};
