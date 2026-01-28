#pragma once
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <memory>
#include "Character.h"

class MetaFile {
public:
   
    MetaFile(const std::string& filename);

    double getSpaceWidth() const;
    const Character* getCharacter(int ascii) const;

private:
    static constexpr int PAD_TOP = 0;
    static constexpr int PAD_LEFT = 1;
    static constexpr int PAD_BOTTOM = 2;
    static constexpr int PAD_RIGHT = 3;
    static constexpr int DESIRED_PADDING = 3;
    static constexpr char SPLITTER = ' ';
    static constexpr char NUMBER_SEPARATOR = ',';

    double aspectRatio;
    double verticalPerPixelSize;
    double horizontalPerPixelSize;
    double spaceWidth;
    std::vector<int> padding;
    int paddingWidth;
    int paddingHeight;

    std::unordered_map<int, Character> metaData;
    std::unique_ptr<std::ifstream> reader;  
    std::unordered_map<std::string, std::string> values;

    bool processNextLine();
    int getValueOfVariable(const std::string& variable) const;
    std::vector<int> getValuesOfVariable(const std::string& variable) const;
    void close();
    void openFile(const std::string& filePath);
    void loadPaddingData();
    void loadLineSizes();
    void loadCharacterData(int imageWidth);
    std::unique_ptr<Character> loadCharacter(int imageSize);
};
