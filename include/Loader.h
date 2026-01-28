
#pragma once
#include <vector>
#include <string>
#include <glad/glad.h>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <memory>
#include <algorithm>
#include "RawModel.h"
#include "TextureData.h"

namespace fs = std::filesystem;

class Loader {
public:
     
    Loader() = default;
    ~Loader() { cleanUp(); }

    int createEmptyVbo(int floatCount);
    void addInstancedAttribute(int vaoId ,int vboId, int attribute, int dataSize ,int instancedDataLength, int offset);
    void updateVbo(int vboId,const std::vector<float>& data);

    RawModel loadToVAO(const std::vector<float>& positions, 
                      const std::vector<float>& textureCoords,
                      const std::vector<float>& normals,
                      const std::vector<int>& indices);
                    
    RawModel loadToVAO(const std::vector<float>& positions, 
                      const std::vector<float>& textureCoords,
                      const std::vector<float>& normals,
                      const std::vector<float>& tangents,
                      const std::vector<int>& indices);

    RawModel loadToVAO(const std::vector<float>& positions , int dimensions);
    
    int loadToVAO(const std::vector<float>& positions , const std::vector<float>& textureCoords);
    
    GLuint loadTexture(const fs::path& fileName,GLenum wrapMode = GL_REPEAT, GLenum filterMode = GL_LINEAR);
    void cleanUp();

    int loadCubeMap(std::vector<std::string>& textureFiles);


private:
    std::vector<GLuint> vaos;
    std::vector<GLuint> vbos;
    std::vector<GLuint> textures;

    GLuint createVAO();
    void storeDataInAttributeList(GLuint attributeNumber, GLint coordinateSize, const std::vector<float>& data);
    void bindIndicesBuffer(const std::vector<int>& indices);
    void unbindVAO();
    TextureData decodeTextureFile(const std::string& fileName);
    // 使用unique_ptr管理临时缓冲区
    std::unique_ptr<float[]> createGLFloatBuffer(const std::vector<float>& data);
    std::unique_ptr<int[]> createGLIntBuffer(const std::vector<int>& data);
};
