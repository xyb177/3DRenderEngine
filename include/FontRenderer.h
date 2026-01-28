#pragma once
#include "FontShader.h"
#include "FontType.h"
#include "GUIText.h"
#include <unordered_map>
/**
 * @class FontRenderer
 * @brief 用于渲染文本的类，使用着色器进行字体渲染
 */
class FontRenderer {
private:
    FontShader shader; ///< 字体渲染使用的着色器

public:
    /**
     * @brief 构造函数，初始化字体着色器
     */
    FontRenderer();

    /**
     * @brief 清理资源
     */
    void cleanUp();

    void render(std::unordered_map<FontType*, std::vector<GUIText*>> texts);

private:
    /**
     * @brief 准备渲染
     */
    void prepare();

    /**
     * @brief 渲染文本
     * @param text 要渲染的GUI文本对象
     */
    void renderText(const GUIText& text);

    /**
     * @brief 结束渲染
     */
    void endRendering();
};
