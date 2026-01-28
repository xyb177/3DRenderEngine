#pragma once
#include "GUIText.h"
#include "FontType.h"
#include "TextMaster.h"

GUIText::GUIText(const std::string& text, 
               float fontSize, 
               FontType* font,
               const glm::vec2& position,
               float maxLineLength,
               bool centered)
    : textString(text),
      fontSize(fontSize),
      font(font),
      position(position),
      lineMaxSize(maxLineLength),
      centerText(centered) 
{   
    TextMaster::getInstance().loadText(*this);
}
    

void GUIText::remove() {
    TextMaster::getInstance().removeText(*this);
}

void GUIText::setColour(float r, float g, float b) {
    colour = glm::vec3(r, g, b);
}

void GUIText::setMeshInfo(unsigned int vao, int verticesCount) {
    textMeshVao = vao;
    vertexCount = verticesCount;
}

void GUIText::setNumberOfLines(int number)
{
    numberOfLines = number;
}
