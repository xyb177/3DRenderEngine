#pragma once

#include "ShaderProgram.h"
#include "glm.hpp"

class ShadowShader : public ShaderProgram {

 
public:
    
    /**
     * 默认构造函数，使用默认着色器文件路径
     */
    ShadowShader();
    
    /**
     * 加载MVP矩阵到着色器
     * @param mvpMatrix 模型-视图-投影矩阵
     */
    void loadMvpMatrix(const glm::mat4& mvpMatrix);

protected:
   
    void getAllUniformLocations() override;

private:
    GLint location_mvpMatrix; // MVP矩阵的统一变量位置

    static const std::string VERTEX_FILE;
    static const std::string FRAGMENT_FILE;
};
