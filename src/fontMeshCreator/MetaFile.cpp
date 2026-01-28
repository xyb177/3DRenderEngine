#include "MetaFile.h"
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include "TextMeshCreator.h"

MetaFile::MetaFile(const std::string& filename) {
    int width, height;
    glfwGetWindowSize(glfwGetCurrentContext(), &width, &height); 
    aspectRatio = static_cast<float>(width) / height;   
    openFile(filename);
    loadPaddingData();
    loadLineSizes();
    int imageWidth = getValueOfVariable("scaleW");
    loadCharacterData(imageWidth);
    close();
}


double MetaFile::getSpaceWidth() const {
    return spaceWidth;
}

const Character* MetaFile::getCharacter(int ascii) const {
    auto it = metaData.find(ascii);
    return it != metaData.end() ? &it->second : nullptr;
}

bool MetaFile::processNextLine() {
    values.clear();
    std::string line;
    if (!std::getline(*reader, line)) {
        return false;
    }

    std::istringstream iss(line);
    std::string pair;
    while (iss >> pair) {
        size_t eqPos = pair.find('=');
        if (eqPos != std::string::npos) {
            std::string key = pair.substr(0, eqPos);
            std::string value = pair.substr(eqPos + 1);
            values[key] = value;
        }
    }
    return true;
}

int MetaFile::getValueOfVariable(const std::string& variable) const {
    auto it = values.find(variable);
    if (it == values.end()) {
        throw std::runtime_error("Variable not found: " + variable);
    }
    return std::stoi(it->second);
}

std::vector<int> MetaFile::getValuesOfVariable(const std::string& variable) const {
    auto it = values.find(variable);
    if (it == values.end()) {
        throw std::runtime_error("Variable not found: " + variable);
    }

    std::vector<int> result;
    std::istringstream iss(it->second);
    std::string token;
    while (std::getline(iss, token, NUMBER_SEPARATOR)) {
        result.push_back(std::stoi(token));
    }
    return result;
}

void MetaFile::close() {
    if (reader && reader->is_open()) {
        reader->close();
    }
}

void MetaFile::openFile(const std::string& filePath) {
    reader = std::make_unique<std::ifstream>(filePath);
    if (!reader->is_open()) {
        throw std::runtime_error("Couldn't read font meta file: " + filePath);
    }
}

void MetaFile::loadPaddingData() {
    if (!processNextLine()) {
        throw std::runtime_error("Failed to read padding data");
    }
    padding = getValuesOfVariable("padding");
    paddingWidth = padding[PAD_LEFT] + padding[PAD_RIGHT];
    paddingHeight = padding[PAD_TOP] + padding[PAD_BOTTOM];
}

void MetaFile::loadLineSizes() {
    if (!processNextLine()) {
        throw std::runtime_error("Failed to read line sizes");
    }
    int lineHeightPixels = getValueOfVariable("lineHeight") - paddingHeight;
    verticalPerPixelSize = TextMeshCreator::LINE_HEIGHT / static_cast<double>(lineHeightPixels);
    horizontalPerPixelSize = verticalPerPixelSize / aspectRatio;
}

void MetaFile::loadCharacterData(int imageWidth) {
    // Skip first two info lines
    if (!processNextLine() || !processNextLine()) {
        throw std::runtime_error("Failed to read character data header");
    }

    while (processNextLine()) {
        auto character = loadCharacter(imageWidth);
        if (character) {
            metaData.emplace(character->getId(), std::move(*character));
        }
    }
}

std::unique_ptr<Character> MetaFile::loadCharacter(int imageSize) {
    int id = getValueOfVariable("id");
    if (id == TextMeshCreator::SPACE_ASCII) {
        spaceWidth = (getValueOfVariable("xadvance") - paddingWidth) * horizontalPerPixelSize;
        return nullptr;
    }

    double xTex = (static_cast<double>(getValueOfVariable("x")) + (padding[PAD_LEFT] - DESIRED_PADDING)) / imageSize;
    double yTex = (static_cast<double>(getValueOfVariable("y")) + (padding[PAD_TOP] - DESIRED_PADDING)) / imageSize;
    int width = getValueOfVariable("width") - (paddingWidth - (2 * DESIRED_PADDING));
    int height = getValueOfVariable("height") - (paddingHeight - (2 * DESIRED_PADDING));
    double quadWidth = width * horizontalPerPixelSize;
    double quadHeight = height * verticalPerPixelSize;
    double xTexSize = static_cast<double>(width) / imageSize;
    double yTexSize = static_cast<double>(height) / imageSize;
    double xOff = (getValueOfVariable("xoffset") + padding[PAD_LEFT] - DESIRED_PADDING) * horizontalPerPixelSize;
    double yOff = (getValueOfVariable("yoffset") + (padding[PAD_TOP] - DESIRED_PADDING)) * verticalPerPixelSize;
    double xAdvance = (getValueOfVariable("xadvance") - paddingWidth) * horizontalPerPixelSize;

    return std::make_unique<Character>(
        id, xTex, yTex, xTexSize, yTexSize, 
        xOff, yOff, quadWidth, quadHeight, xAdvance
    );
}
