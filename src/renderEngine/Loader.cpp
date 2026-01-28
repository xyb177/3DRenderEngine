#pragma once
#include "Loader.h"
#include <glad/glad.h>
#include <algorithm> // for std::copy
#include <stb_image.h>
#include <iostream>

GLuint Loader::createVAO() {
    GLuint vaoID;
    glGenVertexArrays(1, &vaoID);
    vaos.push_back(vaoID);
    glBindVertexArray(vaoID);
    return vaoID;
}

void Loader::storeDataInAttributeList(GLuint attributeNumber, GLint coordinateSize, const std::vector<float>& data) {
    GLuint vboID;
    glGenBuffers(1, &vboID);
    vbos.push_back(vboID);
    
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    auto buffer = createGLFloatBuffer(data);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), buffer.get(), GL_STATIC_DRAW);
    glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, GL_FALSE, 0, nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Loader::bindIndicesBuffer(const std::vector<int>& indices) {
    GLuint vboId;
    glGenBuffers(1, &vboId);
    vbos.push_back(vboId);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboId);
    auto buffer = createGLIntBuffer(indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), buffer.get(), GL_STATIC_DRAW);
}

void Loader::unbindVAO() {
    glBindVertexArray(0);
}

TextureData Loader::decodeTextureFile(const std::string &fileName)
{
    int width, height, channels;
    
    unsigned char* pixels = stbi_load(
        fileName.c_str(), 
        &width, 
        &height, 
        &channels, 
        STBI_rgb_alpha // 强制 4 通道
    );

    if (!pixels) {
        throw std::runtime_error("Failed to load texture: " + fileName);
    }

    std::vector<uint8_t> buffer(pixels, pixels + width * height * 4);
    stbi_image_free(pixels); 

    return TextureData(std::move(buffer), width, height);
}

std::unique_ptr<float[]> Loader::createGLFloatBuffer(const std::vector<float>& data) {
    auto buffer = std::make_unique<float[]>(data.size());
    std::copy(data.begin(), data.end(), buffer.get());
    return buffer;
}

std::unique_ptr<int[]> Loader::createGLIntBuffer(const std::vector<int>& data) {
    auto buffer = std::make_unique<int[]>(data.size());
    std::copy(data.begin(), data.end(), buffer.get());
    return buffer;
}

int Loader::createEmptyVbo(int floatCount)
{
    GLuint vboId;
    glGenBuffers(1, &vboId);
    vbos.push_back(vboId);
    
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, floatCount * sizeof(float),nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return vboId;
}

void Loader::addInstancedAttribute(int vaoId, int vboId, int attribute, int dataSize, int instancedDataLength, int offset)
{
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBindVertexArray(vaoId);
    glVertexAttribPointer(attribute, dataSize, GL_FLOAT, GL_FALSE, instancedDataLength * sizeof(float),(void*)(offset * sizeof(float)));
    glVertexAttribDivisor(attribute,1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Loader::updateVbo(int vboId, const std::vector<float> &data)
{
    auto buffer = createGLFloatBuffer(data);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), buffer.get(), GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0,data.size() * sizeof(float), buffer.get()); 
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

RawModel Loader::loadToVAO(const std::vector<float> &positions,
                           const std::vector<float> &textureCoords,
                           const std::vector<float> &normals,
                           const std::vector<int> &indices)
{
    GLuint vaoID = createVAO();
    bindIndicesBuffer(indices);
    storeDataInAttributeList(0, 3, positions);
    storeDataInAttributeList(1, 2, textureCoords);
    storeDataInAttributeList(2, 3, normals);
    unbindVAO();
    return RawModel(vaoID, static_cast<GLuint>(indices.size()));
}

RawModel Loader::loadToVAO(const std::vector<float> &positions, 
                          const std::vector<float> &textureCoords, 
                          const std::vector<float> &normals, 
                          const std::vector<float> &tangents, 
                          const std::vector<int> &indices)
{
    GLuint vaoID = createVAO();
    bindIndicesBuffer(indices);
    storeDataInAttributeList(0, 3, positions);
    storeDataInAttributeList(1, 2, textureCoords);
    storeDataInAttributeList(2, 3, normals);
    storeDataInAttributeList(3, 3, tangents);
    unbindVAO();
    return RawModel(vaoID, static_cast<GLuint>(indices.size()));
}

RawModel Loader::loadToVAO(const std::vector<float> &positions, int dimensions)
{   
    int vaoID = createVAO();
    storeDataInAttributeList(0,dimensions,positions);
    unbindVAO();
    return RawModel(vaoID, static_cast<GLuint>(positions.size()/dimensions));
}

int Loader::loadToVAO(const std::vector<float> &positions, const std::vector<float> &textureCoords)
{
    GLuint vaoID = createVAO();
    storeDataInAttributeList(0, 2, positions);
    storeDataInAttributeList(1, 2, textureCoords);
    unbindVAO();
    return static_cast<int>(vaoID);
}

GLuint Loader::loadTexture(const fs::path &fileName,GLenum wrapMode, GLenum filterMode)
{
    GLuint textureID;
    glGenTextures(1,&textureID);

    int width,height, nrComponents;
    unsigned char* data = stbi_load(fileName.string().c_str(), &width, &height, &nrComponents, 0);

    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 0);
        stbi_image_free(data);
    } else {
        std::cerr << "Texture failed to load at path: " << fileName << std::endl;
        stbi_image_free(data);
        throw std::runtime_error("Failed to load texture");
    }

    if (GLAD_GL_EXT_texture_filter_anisotropic) {
        // 获取硬件支持的最大各向异性级别
        GLfloat maxSupported;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxSupported);
        
        // 计算实际使用的各向异性级别
        float amount = std::min(4.0f, maxSupported);
        
        // 设置各向异性过滤参数
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, amount);
    } else {
        std::cout<< "Not supported" << std::endl;
    }
    textures.push_back(textureID);
    return textureID;
}

void Loader::cleanUp()
{
    if (!vaos.empty()) {
        glDeleteVertexArrays(static_cast<GLsizei>(vaos.size()), vaos.data());
        vaos.clear();
    }
    
    if (!vbos.empty()) {
        glDeleteBuffers(static_cast<GLsizei>(vbos.size()), vbos.data());
        vbos.clear();
    }
    if (!textures.empty()) {
        glDeleteTextures(static_cast<GLsizei>(textures.size()), textures.data());
        textures.clear();
    }
}

int Loader::loadCubeMap(std::vector<std::string> &textureFiles)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    for( int i=0;i<textureFiles.size();i++)
    {
        TextureData data = decodeTextureFile(textureFiles[i]);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,0,GL_RGBA,
                        data.getWidth(),data.getHeight(),0,GL_RGBA,GL_UNSIGNED_BYTE,data.getBuffer().data());
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    textures.push_back(textureID);
    return textureID;
}
