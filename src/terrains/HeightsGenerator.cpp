// HeightsGenerator.cpp
#include "HeightsGenerator.h"
#include <functional>

/**
 * 默认构造函数 - 使用随机种子初始化
 */
HeightsGenerator::HeightsGenerator() {
    std::random_device rd;
    seed = std::uniform_int_distribution<int>(0, 1000000000)(rd);
    random.seed(seed);
}

/**
 * 带参数构造函数 - 用于分块地形生成
 * @param gridX, gridZ: 网格坐标（必须为正数）
 * @param vertexCount: 顶点数量
 * @param seed: 随机种子
 */
HeightsGenerator::HeightsGenerator(int gridX, int gridZ, int vertexCount, int seed) 
    : seed(seed), random(seed) {
    xOffset = gridX * (vertexCount - 1);
    zOffset = gridZ * (vertexCount - 1);
}

/**
 * 生成高度值 - 多层级噪声叠加
 * @param x, z: 世界坐标
 * @return: 对应位置的高度值
 */
float HeightsGenerator::generateHeight(int x, int z) {
    float total = 0.0f;
    float d = std::pow(2.0f, OCTAVES - 1);
    
    for (int i = 0; i < OCTAVES; i++) {
        float freq = std::pow(2.0f, i) / d ;
        float amp = std::pow(ROUGHNESS, i) * AMPLITUDE;
        total += getInterpolatedNoise((x + xOffset) * freq, (z + zOffset) * freq) * amp;
    }
    return total;
}

/**
 * 双线性插值噪声 - 在四个噪声点之间进行插值
 */
float HeightsGenerator::getInterpolatedNoise(float x, float z) {
    int intX = static_cast<int>(x);
    int intZ = static_cast<int>(z);
    float fracX = x - intX;
    float fracZ = z - intZ;
    
    float v1 = getSmoothNoise(intX, intZ);
    float v2 = getSmoothNoise(intX + 1, intZ);
    float v3 = getSmoothNoise(intX, intZ + 1);
    float v4 = getSmoothNoise(intX + 1, intZ + 1);
    
    float i1 = interpolate(v1, v2, fracX);
    float i2 = interpolate(v3, v4, fracX);
    return interpolate(i1, i2, fracZ);
}

/**
 * 余弦插值 - 使用cos函数实现平滑过渡
 */
float HeightsGenerator::interpolate(float a, float b, float blend) {
    double theta = blend * M_PI;
    float f = (1.0f - std::cos(theta)) * 0.5f;
    return a * (1.0f - f) + b * f;
}

/**
 * 平滑噪声 - 3x3区域加权平均
 * 使用 corners(1/16) + sides(1/8) + center(1/4) 的权重
 */
float HeightsGenerator::getSmoothNoise(int x, int z) {
    float corners = (getNoise(x - 1, z - 1) + getNoise(x + 1, z - 1) + 
                    getNoise(x - 1, z + 1) + getNoise(x + 1, z + 1)) / 16.0f;
    
    float sides = (getNoise(x - 1, z) + getNoise(x + 1, z) + 
                  getNoise(x, z - 1) + getNoise(x, z + 1)) / 8.0f;
    
    float center = getNoise(x, z) / 4.0f;
    
    return corners + sides + center;
}

/**
 * 基础噪声生成 - 基于坐标和种子生成伪随机数
 * 返回范围：[-1.0, 1.0]
 */
float HeightsGenerator::getNoise(int x, int z) {
    // 使用哈希函数确保不同坐标产生不同随机数
    std::size_t hash = std::hash<int>{}(x * 49632 + z * 325176 + seed);
        
        // 检查缓存
        auto it = noiseCache.find(hash);
        if (it != noiseCache.end()) {
            return it->second;
        }
        
        // 使用固定的随机数生成器状态
        std::mt19937 localRandom(hash);
        float noise = dist(localRandom);
        
        // 缓存结果
        noiseCache[hash] = noise;
        return noise;
}
