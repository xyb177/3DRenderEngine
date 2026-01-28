#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath) {
    vertexShaderID = loadShader(vertexPath, GL_VERTEX_SHADER);
    fragmentShaderID = loadShader(fragmentPath, GL_FRAGMENT_SHADER);
    programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);


    glLinkProgram(programID);
    checkShaderCompileErrors(programID, "PROGRAM");

    glValidateProgram(programID);
    
}    

ShaderProgram::~ShaderProgram() {
    cleanUp();
}

void ShaderProgram::initUniforms()
{
    getAllUniformLocations();
}

void ShaderProgram::start() const
{
    glUseProgram(programID);
}

void ShaderProgram::stop() const {
    glUseProgram(0);
}

void ShaderProgram::cleanUp() {
    stop();
    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
    glDeleteProgram(programID);
}

void ShaderProgram::loadInt(GLuint location, int value) const
{
    glUniform1i(location, value);
}

void ShaderProgram::loadFloat(GLuint location, float value) const
{
    glUniform1f(location, value);
}

void ShaderProgram::loadVector(GLuint location, const glm::vec3& vector) const {
    glUniform3f(location, vector.x, vector.y, vector.z);
}

void ShaderProgram::loadVector(GLuint location, const glm::vec4 &vector) const
{
    glUniform4f(location, vector.x, vector.y, vector.z,vector.w);
}

void ShaderProgram::load2DVector(GLuint location, const glm::vec2 &vector) const
{
    glUniform2f(location, vector.x, vector.y);
}

void ShaderProgram::loadBoolean(GLuint location, bool value) const {
    glUniform1f(location, value ? 1.0f : 0.0f);
}

void ShaderProgram::loadMatrix(GLuint location, const glm::mat4& matrix) const {
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

GLuint ShaderProgram::getUniformLocation(const std::string& uniformName) const {
    return glGetUniformLocation(programID, uniformName.c_str());
}

GLuint ShaderProgram::loadShader(const std::string& shaderPath, GLenum shaderType) {
    std::string shaderCode;
    std::ifstream shaderFile;
    
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        shaderFile.open(shaderPath);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        shaderCode = shaderStream.str();
    } catch (std::ifstream::failure& e) {
        throw std::runtime_error("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " + shaderPath);
    }
    
    const char* shaderCodeCStr = shaderCode.c_str();
    GLuint shaderID = glCreateShader(shaderType);
    glShaderSource(shaderID, 1, &shaderCodeCStr, nullptr);
    glCompileShader(shaderID);
    checkShaderCompileErrors(shaderID, "SHADER");
   
    return shaderID;
}

void ShaderProgram::checkShaderCompileErrors(GLuint shader, const std::string& type) {
    GLint success;
    GLchar infoLog[1024];
    
    if (type == "PROGRAM") {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            throw std::runtime_error("ERROR::PROGRAM_LINKING_ERROR\n" + std::string(infoLog));
        }
    } else {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            throw std::runtime_error("ERROR::" + type + "_COMPILATION_ERROR\n" + std::string(infoLog));
        }
    }
}
