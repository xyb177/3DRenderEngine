#pragma once
#include <vector>
#include <cstdint> // 用于明确整数类型

class TextureData {
public:
    // 构造函数：接受像素数据和尺寸
    TextureData(std::vector<uint8_t> buffer, int width, int height);

    // 获取纹理宽度
    int getWidth() const;

    // 获取纹理高度
    int getHeight() const;

    // 获取像素数据（返回 const 引用避免拷贝）
    const std::vector<uint8_t>& getBuffer() const;

private:
    int width;
    int height;
    std::vector<uint8_t> buffer; // 使用 vector 存储像素数据
};
