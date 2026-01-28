#include "TextureData.h"

// 构造函数
TextureData::TextureData(std::vector<uint8_t> buffer, int width, int height)
    : buffer(std::move(buffer)), width(width), height(height) {}

// 获取宽度
int TextureData::getWidth() const {
    return width;
}

// 获取高度
int TextureData::getHeight() const {
    return height;
}

// 获取像素数据
const std::vector<uint8_t>& TextureData::getBuffer() const {
    return buffer;
}
