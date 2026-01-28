#pragma once
#include "ImageRenderer.h"
#include "HorizontalBlurShader.h"



/**
 * @brief 水平高斯模糊处理器
 * 对输入纹理进行水平方向的高斯模糊处理
 */
class HorizontalBlur {
public:
    /**
     * @brief 构造函数
     * @param targetFboWidth 目标帧缓冲宽度
     * @param targetFboHeight 目标帧缓冲高度
     */
    HorizontalBlur(int targetFboWidth, int targetFboHeight);
    
    
    /**
     * @brief 析构函数
     */
    ~HorizontalBlur() = default;

    /**
     * @brief 对输入纹理进行水平模糊处理
     * @param texture 输入纹理ID
     */
    void render(unsigned int texture);

    /**
     * @brief 获取处理后的输出纹理
     * @return 输出纹理ID
     */
    unsigned int getOutputTexture() const;

    /**
     * @brief 清理资源
     */
    void cleanUp();

private:
    ImageRenderer renderer; // 图像渲染器
    HorizontalBlurShader shader; // 水平模糊着色器
};
