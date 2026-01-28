#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include "glm.hpp"
#include "gtc/type_ptr.hpp"
#include <vector>

class ShaderProgram {


public:
    ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);
    virtual ~ShaderProgram();

    
    
    void initUniforms();

    void start() const;
    void stop() const;
    void cleanUp();

protected:
    GLuint programID;
    GLuint vertexShaderID;
    GLuint fragmentShaderID;

    virtual void getAllUniformLocations()=0;
     
    void loadInt(GLuint location, int value) const;
    void loadFloat(GLuint location, float value) const;
    void loadVector(GLuint location, const glm::vec3& vector) const;
    void loadVector(GLuint location, const glm::vec4& vector) const;
    void load2DVector(GLuint location, const glm::vec2& vector) const;
    void loadBoolean(GLuint location, bool value) const;
    void loadMatrix(GLuint location, const glm::mat4& matrix) const;
    
    GLuint getUniformLocation(const std::string& uniformName) const;

private:
    static GLuint loadShader(const std::string& shaderPath, GLenum shaderType);
    static void checkShaderCompileErrors(GLuint shader, const std::string& type);
};
