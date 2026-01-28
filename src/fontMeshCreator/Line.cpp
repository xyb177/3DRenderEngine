#include "Line.h"

Line::Line(double spaceWidth, double fontSize, double maxLength)
    : maxLength(maxLength),
      spaceSize(spaceWidth * fontSize) {}

bool Line::attemptToAddWord(const Word& word) {
    double additionalLength = word.getWordWidth();
    additionalLength += !words.empty() ? spaceSize : 0;
    
    if (currentLineLength + additionalLength <= maxLength) {
        words.push_back(word);
        currentLineLength += additionalLength;
        return true;
    }
    return false;
}
