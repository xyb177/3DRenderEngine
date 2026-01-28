// HeightsGenerator.h
#pragma once

#include <random>
#include <cmath>
#include <vector>
#include <unordered_map>

/**
 * 高度图生成器 - 使用Perlin噪声生成地形高度
 * 功能：基于网格坐标生成平滑的高度值，用于地形生成
 */
class HeightsGenerator {
private:
    static constexpr float AMPLITUDE = 30.0f;    // 振幅，控制高度范围
    static constexpr int OCTAVES = 3;            // 噪声层数，影响细节复杂度
    static constexpr float ROUGHNESS = 0.3f;     // 粗糙度，控制每层噪声的影响程度

    std::mt19937 random;                         // C++11随机数生成器
    int seed;                                    // 随机种子
    int xOffset = 0;                             // X轴偏移量
    int zOffset = 0;                             // Z轴偏移量


    mutable std::unordered_map<std::size_t, float> noiseCache;
    
    // 预创建的分布对象
    std::uniform_real_distribution<float> dist{-1.0f, 1.0f};
    // 获取基础噪声值
    float getNoise(int x, int z);
    
    // 获取平滑噪声值（3x3区域平均）
    float getSmoothNoise(int x, int z);
    
    // 双线性插值噪声
    float getInterpolatedNoise(float x, float z);
    
    // 余弦插值函数
    float interpolate(float a, float b, float blend);

public:
    // 默认构造函数
    HeightsGenerator();
    
    // 带参数的构造函数（仅支持正数gridX和gridZ）
    HeightsGenerator(int gridX, int gridZ, int vertexCount, int seed);
    
    // 生成指定位置的高度值
    float generateHeight(int x, int z);
};
