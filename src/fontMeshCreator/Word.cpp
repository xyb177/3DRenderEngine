#include "Word.h"

Word::Word(double fontSize) : fontSize(fontSize) {}

void Word::addCharacter(const Character& character) {
    characters.push_back(character);
    width += character.getxAdvance() * fontSize;
}
