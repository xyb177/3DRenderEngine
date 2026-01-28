#pragma once
#include "glm.hpp"
#include "Player.h"
#include <GLFW/glfw3.h>

class Camera {
public:
    Camera(Player& player);
    
    void move();
    
    const glm::vec3& getPosition() const;
    float getPitch() const;
    float getYaw() const ;
    float getRoll() const;

    void setSpeed(float n);
    void setPosition(float x, float y,float z);
    void invertPitch();
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

private:
    void calculateZoom();
    void calculatePitch();
    void calculateAngleAroundPlayer();

    float calculateHorizontalDistance();
    float calculateVerticalDistance();
    
    void calculateCameraPosition(float horizDistance,float verticDistance);

private:
    glm::vec3 position = glm::vec3(0.0f,50.0f,0.0f);
    float pitch = 30.0f;  // 俯仰角 (X轴旋转)
    float yaw = 0.0f;    // 偏航角 (Y轴旋转)
    float roll = 0.0f;   // 滚转角 (Z轴旋转)
    
    float moveSpeed = 0.1f;

    float distanceFromPlayer = 50;
    float angleAroundPlayer = 0;    

    Player& player;
    GLFWwindow* window;

    static inline double scrollY = 0.0;

    bool firstMousePressPitch = true;
    bool firstMousePressYaw = true;
    double lastMouseX = 0.0;
    double lastMouseY = 0.0;
};
