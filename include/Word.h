#pragma once
#include <vector>
#include "Character.h"

class Word {
public:
    /**
     * Create a new empty word
     * @param fontSize - the font size of the text
     */
    Word(double fontSize);

    /**
     * Adds a character to the word
     * @param character - the character to add
     */
    void addCharacter(const Character& character);

    // Getters
    const std::vector<Character>& getCharacters() const noexcept { return characters; }
    double getWordWidth() const noexcept { return width; }

private:
    std::vector<Character> characters;
    double width = 0;
    double fontSize;
};
