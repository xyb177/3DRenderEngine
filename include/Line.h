#pragma once
#include <vector>
#include <memory>
#include "Word.h"

class Line {
public:
    /**
     * Creates an empty line.
     * 
     * @param spaceWidth - the screen-space width of a space character
     * @param fontSize - the size of font being used
     * @param maxLength - the screen-space maximum length of a line
     */
    Line(double spaceWidth, double fontSize, double maxLength);

    /**
     * Attempt to add a word to the line.
     * 
     * @param word - the word to try to add
     * @return true if the word was successfully added
     */
    bool attemptToAddWord(const Word& word);

    // Getters
    const double getMaxLength() const noexcept { return maxLength; }
    const double getLineLength() const noexcept { return currentLineLength; }
    const std::vector<Word>& getWords() const noexcept { return words; }

private:
    double maxLength;
    double spaceSize;
    std::vector<Word> words;
    double currentLineLength = 0;
};
