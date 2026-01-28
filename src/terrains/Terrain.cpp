#include "Terrain.h"

Terrain::Terrain(int gridX, int gridZ, Loader &loader, const TerrainTexturePack &texturePack, 
                    const TerrainTexture &blendMap, const std::string &heightMap)
                : texturePack(texturePack),blendMap(blendMap), x(gridX * SIZE), z(gridZ * SIZE), 
                    model(generateTerrain(loader,heightMap))
{
}

Terrain::Terrain(int gridX, int gridZ, Loader &loader, const TerrainTexturePack &texturePack, 
                    const TerrainTexture &blendMap)
                : texturePack(texturePack), blendMap(blendMap), x(gridX * SIZE), z(gridZ * SIZE),
                  model(generateProceduralTerrain(loader, gridX, gridZ))
{
}

float Terrain::getHeightOfTerrain(float worldX, float worldZ)
{   
    if (m_heightData.empty() || m_heightData[0].empty()) {
        return 0.0f;
    }
    float terrainX = worldX - this->x;
    float terrainZ = worldZ - this->z;
    float gridSquareSize = this->SIZE /static_cast<float>(m_heightData.size() - 1);
    int gridX = static_cast<int>(std::floor(terrainX / gridSquareSize));
    int gridZ = static_cast<int>(std::floor(terrainZ / gridSquareSize));
    if (gridX >=m_heightData[0].size()-1 || gridZ >=m_heightData.size()-1 || gridX < 0 || gridZ < 0)
    {
        return 0;
    }

    float xCoord = std::fmod(terrainX, gridSquareSize) /gridSquareSize;
    float zCoord = std::fmod(terrainZ, gridSquareSize) /gridSquareSize;

    float answer;
    if (xCoord <=(1 - zCoord)){
        answer = Maths::barryCentric(
            glm::vec3(0,m_heightData[gridZ][gridX],0),
            glm::vec3(1,m_heightData[gridZ][gridX+1],0),
            glm::vec3(0,m_heightData[gridZ+1][gridX],1),
            glm::vec2(xCoord,zCoord)
        );
    } else {
        answer = Maths::barryCentric(
            glm::vec3(1,m_heightData[gridZ][gridX+1],0),
            glm::vec3(1,m_heightData[gridZ+1][gridX+1],1),
            glm::vec3(0,m_heightData[gridZ+1][gridX],1),
            glm::vec2(xCoord,zCoord)
        );
    }

    return answer;
}

RawModel Terrain::generateTerrain(Loader &loader, std::string heightMap)
{   
    int width, height, channels;
    unsigned char* imageData = stbi_load(heightMap.c_str(), &width, &height, &channels, 0);
    
    std::vector<unsigned char> imageBuffer;
    if (imageData) {
        imageBuffer.assign(imageData, imageData + width * height * channels);
        stbi_image_free(imageData);
        loadHeightMap(imageBuffer, width, height, channels);
    } else {
        std::cout << "Height map not found, using procedural generation instead" << std::endl;
        return generateProceduralTerrain(loader, x / SIZE, z / SIZE);
    }

    return generateMeshFromHeightData(loader, width, height);
}


RawModel Terrain::generateProceduralTerrain(Loader &loader, int gridX, int gridZ)
{
    const int VERTEX_COUNT = 64;
    
    // 初始化高度生成器
    HeightsGenerator generator;
    
    // 生成程序化高度数据
    generateProceduralHeightData(VERTEX_COUNT, generator);
    
    return generateMeshFromHeightData(loader, VERTEX_COUNT, VERTEX_COUNT);
}

void Terrain::generateProceduralHeightData(int size, HeightsGenerator& generator)
{
    m_heightData.resize(size);
    for (int z = 0; z < size; ++z) {
        m_heightData[z].resize(size);
        for (int x = 0; x < size; ++x) {
            m_heightData[z][x] = generator.generateHeight(x, z);
        }
    }
}


RawModel Terrain::generateMeshFromHeightData(Loader &loader, int width, int height)
{
    const int VERTEX_COUNT = width; 
    const int count = VERTEX_COUNT * VERTEX_COUNT;
    
    std::vector<float> vertices(count * 3);
    std::vector<float> normals(count * 3);
    std::vector<float> textureCoords(count * 2);
    std::vector<int> indices(6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1));

    // 生成顶点数据
    for (int i = 0; i < VERTEX_COUNT; ++i) {
        for (int j = 0; j < VERTEX_COUNT; ++j) {
            int vertexPointer = i * VERTEX_COUNT + j;
            
            vertices[vertexPointer * 3] = static_cast<float>(j) / (VERTEX_COUNT - 1) * SIZE;
            vertices[vertexPointer * 3 + 1] = m_heightData[i][j]; // 使用预先计算的高度数据
            vertices[vertexPointer * 3 + 2] = static_cast<float>(i) / (VERTEX_COUNT - 1) * SIZE;
            
            glm::vec3 normal = calculateNormal(j, i);
            normals[vertexPointer * 3] = normal.x;
            normals[vertexPointer * 3 + 1] = normal.y;
            normals[vertexPointer * 3 + 2] = normal.z;
            
            textureCoords[vertexPointer * 2] = static_cast<float>(j) / (VERTEX_COUNT - 1);
            textureCoords[vertexPointer * 2 + 1] = static_cast<float>(i) / (VERTEX_COUNT - 1);
        }
    }

    // 生成索引数据
    int pointer = 0;
    for (int gz = 0; gz < VERTEX_COUNT - 1; ++gz) {
        for (int gx = 0; gx < VERTEX_COUNT - 1; ++gx) {
            int topLeft = (gz * VERTEX_COUNT) + gx;
            int topRight = topLeft + 1;
            int bottomLeft = ((gz + 1) * VERTEX_COUNT) + gx;
            int bottomRight = bottomLeft + 1;
            
            indices[pointer++] = topLeft;
            indices[pointer++] = bottomLeft;
            indices[pointer++] = topRight;
            indices[pointer++] = topRight;
            indices[pointer++] = bottomLeft;
            indices[pointer++] = bottomRight;
        }
    }

    return loader.loadToVAO(vertices, textureCoords, normals, indices);
}


void Terrain::loadHeightMap(const std::vector<unsigned char> &imageData, int width, int height, int channels)
{
    auto [minIt, maxIt] = std::minmax_element(imageData.begin(), imageData.end());
    m_minHeight = static_cast<float>(*minIt);
    m_maxHeight = static_cast<float>(*maxIt);
    float range = m_maxHeight - m_minHeight;

    m_heightData.resize(height);
    for (int z = 0; z < height; ++z) {
        m_heightData[z].resize(width);
        for (int x = 0; x < width; ++x) {
            int idx = (z * width + x) * channels;
            float pixelValue = imageData[idx];
                
            if (channels >= 3) {
                pixelValue = (imageData[idx] + imageData[idx+1] + imageData[idx+2]) / 3.0f;
            }
                
            m_heightData[z][x] = ((pixelValue - m_minHeight) / range) * MAX_HEIGHT * 2;
        }
    }
}

float Terrain::getHeight(int x, int z)
{
    // 安全检查
    if (m_heightData.empty() || 
        z < 0 || z >= static_cast<int>(m_heightData.size()) ||
        x < 0 || x >= static_cast<int>(m_heightData[z].size())) {
        return 0.0f;
    }
    return m_heightData[z][x];
}

glm::vec3 Terrain::calculateNormal(int x, int z)
{
    // 安全检查
    if (m_heightData.empty()) {
        return glm::vec3(0, 1, 0); // 默认法线
    }

    float heightL = (x > 0) ? getHeight(x-1, z) : getHeight(x, z);
    float heightR = (x < static_cast<int>(m_heightData[0].size())-1) ? getHeight(x+1, z) : getHeight(x, z);
    float heightD = (z > 0) ? getHeight(x, z-1) : getHeight(x, z);
    float heightU = (z < static_cast<int>(m_heightData.size())-1) ? getHeight(x, z+1) : getHeight(x, z);
    
    glm::vec3 normal(heightL - heightR, 2.0f, heightD - heightU);
    return glm::normalize(normal);
}

// RawModel Terrain::generateTerrain(Loader &loader,std::string heightMap)
// {   

//     HeightsGenerator generator;

//     int width, height, channels;
//     unsigned char* imageData = stbi_load(heightMap.c_str(), &width, &height, &channels, 0);
    
//     std::vector<unsigned char> imageBuffer;
//     if (imageData) {
//         imageBuffer.assign(imageData, imageData + width * height * channels);
//         stbi_image_free(imageData);
//     } else {
//         const char* reason = stbi_failure_reason();
//         throw std::runtime_error("Failed to load height map: " + std::string(reason));
//     }

//     loadHeightMap(imageBuffer,width,height,channels);

//     int VERTEX_COUNT = height;
//     //int VERTEX_COUNT = 128;
//     const int count = VERTEX_COUNT * VERTEX_COUNT;
    
//     std::vector<float> vertices(count * 3);
//     std::vector<float> normals(count * 3);
//     std::vector<float> textureCoords(count * 2);
//     std::vector<int> indices(6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1));

//     // 生成顶点数据
//     for (int i = 0; i < VERTEX_COUNT; ++i) {
//         for (int j = 0; j < VERTEX_COUNT; ++j) {
//             int vertexPointer = i * VERTEX_COUNT + j;
            
//             vertices[vertexPointer * 3] = static_cast<float>(j) / (VERTEX_COUNT - 1) * SIZE;
//             vertices[vertexPointer * 3 + 1] = getHeight(j,i,width,height);
//             //vertices[vertexPointer * 3 + 1] = getHeight(j,i,generator);
//             vertices[vertexPointer * 3 + 2] = static_cast<float>(i) / (VERTEX_COUNT - 1) * SIZE;
            
//             glm::vec3 normal = calculateNormal(j,i,width,height);

//             normals[vertexPointer * 3] = normal.x;
//             normals[vertexPointer * 3 + 1] = normal.y;
//             normals[vertexPointer * 3 + 2] = normal.z;
            
//             textureCoords[vertexPointer * 2] = static_cast<float>(j) / (VERTEX_COUNT - 1);
//             textureCoords[vertexPointer * 2 + 1] = static_cast<float>(i) / (VERTEX_COUNT - 1);
//         }
//     }

//     // 生成索引数据
//     int pointer = 0;
//     for (int gz = 0; gz < VERTEX_COUNT - 1; ++gz) {
//         for (int gx = 0; gx < VERTEX_COUNT - 1; ++gx) {
//             int topLeft = (gz * VERTEX_COUNT) + gx;
//             int topRight = topLeft + 1;
//             int bottomLeft = ((gz + 1) * VERTEX_COUNT) + gx;
//             int bottomRight = bottomLeft + 1;
            
//             indices[pointer++] = topLeft;
//             indices[pointer++] = bottomLeft;
//             indices[pointer++] = topRight;
//             indices[pointer++] = topRight;
//             indices[pointer++] = bottomLeft;
//             indices[pointer++] = bottomRight;
//         }
//     }

//     return loader.loadToVAO(vertices, textureCoords, normals, indices);
// }

// void Terrain::loadHeightMap(const std::vector<unsigned char> &imageData, int width, int height, int channels)
// {
//     auto [minIt, maxIt] = std::minmax_element(imageData.begin(), imageData.end());
//     m_minHeight = static_cast<float>(*minIt);
//     m_maxHeight = static_cast<float>(*maxIt);
//     float range = m_maxHeight - m_minHeight;

//     m_heightData.resize(height);
//     for (int z = 0; z < height; ++z) {
//         for (int x = 0; x < width; ++x) {
//             int idx = (z * width + x) * channels;
//             float pixelValue = imageData[idx];
                
//             if (channels >= 3) {  // RGB转灰度
//                 pixelValue = (imageData[idx] + imageData[idx+1] + imageData[idx+2]) / 3.0f;
//             }
                
//             m_heightData[z].push_back(((pixelValue - m_minHeight) / range) * MAX_HEIGHT*2);
//         }
//     }
// }

// float Terrain::getHeight(int x, int z, int width, int height) {
//     if (x < 0 || x >= width || z < 0 || z >= height) return 0.0f;
//         return m_heightData[z][x];
// }

// float Terrain::getHeight(int x, int z, HeightsGenerator& generator)
// {
//     return generator.generateHeight(x,z);
// }

// glm::vec3 Terrain::calculateNormal(int x ,int z, int width, int height)
// {   float heightL = getHeight(x-1,z,width,height);
//     float heightR = getHeight(x+1,z,width,height);
//     float heightD = getHeight(x,z-1,width,height);
//     float heightU = getHeight(x,z+1,width,height);
//     glm::vec3 normal(heightL - heightR,2.0f,heightD-heightU);
//     return glm::normalize(normal);
// }

// glm::vec3 Terrain::calculateNormal(int x, int z, HeightsGenerator& generator)
// {
//     float heightL = getHeight(x-1,z,generator);
//     float heightR = getHeight(x+1,z,generator);
//     float heightD = getHeight(x,z-1,generator);
//     float heightU = getHeight(x,z+1,generator);
//     glm::vec3 normal(heightL - heightR,2.0f,heightD-heightU);
//     return glm::normalize(normal);
// }
