#pragma once

#include "glm.hpp"
#include <random>
#include <memory>
#include "gtc/constants.hpp"
#include "gtc/matrix_transform.hpp"
#include "Particle.h"
#include "DisplayManager.h"

// 粒子系统类，用于生成和管理粒子效果
class ParticleSystem {

public:
    /**
     * 粒子系统构造函数
     * @param pps 每秒生成粒子数
     * @param speed 平均速度
     * @param gravityComplient 重力系数
     * @param lifeLength 平均生命周期
     * @param scale 平均缩放比例
     */
    ParticleSystem(ParticleTexture texture, float pps, float speed, float gravityComplient, 
                   float lifeLength, float scale);

    /**
     * 设置粒子发射方向
     * @param direction 平均发射方向
     * @param deviation 偏差值(0-1)，表示粒子可以偏离选定方向的程度
     */
    void setDirection(const glm::vec3& direction, float deviation);

    // 启用随机旋转
    void randomizeRotation();

    /**
     * 设置速度误差范围
     * @param error 误差值(0-1)，0表示无误差
     */
    void setSpeedError(float error);

    /**
     * 设置生命周期误差范围
     * @param error 误差值(0-1)，0表示无误差
     */
    void setLifeError(float error);

    /**
     * 设置缩放误差范围
     * @param error 误差值(0-1)，0表示无误差
     */
    void setScaleError(float error);

    /**
     * 生成粒子
     * @param systemCenter 系统中心位置
     * @param deltaTime 帧时间
     */
    void generateParticles(const glm::vec3& systemCenter);

private:
    // 发射单个粒子
    void emitParticle(const glm::vec3& center);
    
    // 生成带误差的值
    float generateValue(float average, float errorMargin);
    
    // 生成旋转角度
    float generateRotation();
    
    // 在锥形范围内生成随机单位向量
    glm::vec3 generateRandomUnitVectorWithinCone(const glm::vec3& coneDirection, float angle);
    
    // 生成随机单位向量
    glm::vec3 generateRandomUnitVector();

private:
    float pps; // 每秒生成粒子数
    float averageSpeed; // 平均速度
    float gravityComplient; // 重力系数
    float averageLifeLength; // 平均生命周期
    float averageScale; // 平均缩放比例

    float speedError = 0.0f; // 速度误差范围
    float lifeError = 0.0f; // 生命周期误差范围
    float scaleError = 0.0f; // 缩放误差范围
    bool randomRotation = false; // 是否随机旋转
    glm::vec3 direction; // 发射方向
    float directionDeviation = 0.0f; // 方向偏差

    std::mt19937 randomEngine; // 随机数引擎
    std::uniform_real_distribution<float> randomDist; // 均匀分布随机数

    ParticleTexture texture;
};
