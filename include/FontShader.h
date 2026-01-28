#pragma once
#include "ShaderProgram.h"
#include <string>

/**
 * @class FontShader
 * @brief 字体渲染专用的着色器程序
 */
class FontShader : public ShaderProgram {

public:
    /**
     * @brief 构造函数，加载并编译着色器
     */
    FontShader();
    void loadColour(glm::vec3 colour);
    void loadTranslation(glm::vec2 translation);
protected:
    /**
     * @brief 获取所有uniform变量的位置
     */
    void getAllUniformLocations() override;

private:
    static const std::string& getVertexFile();
    static const std::string& getFragmentFile();

    int location_colour;
    int location_translation;

};
