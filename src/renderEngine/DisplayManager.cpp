#pragma once
#include "DisplayManager.h"
#include <windows.h>

DisplayManager::DisplayManager():window(nullptr), LastFrameTime(0), FrameTime(0){}


DisplayManager &DisplayManager::getInstance()
{
    static DisplayManager instance;
    return instance;
}

bool DisplayManager::CreateDisplay()
{   //该函数用于初始化GLFW库
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
            return false;
    }
    //配置GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // glfwWindowHint(GLFW_SAMPLES, 4);
    //创建窗口
    window = glfwCreateWindow(width, height, Title, nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    glViewport(0, 0, width, height);

    LastFrameTime = glfwGetTime();
    return true;
}

void DisplayManager::UpdateDisplay()
{
    double CurrentTime = glfwGetTime();
    FrameTime = CurrentTime - LastFrameTime;
    LastFrameTime = CurrentTime;

    if (FrameTime < 1.0 / FPS_CAP) {
        double SleepTime = (1.0 / FPS_CAP - FrameTime) * 1000;
        Sleep(SleepTime);
    }
        
        // 交换缓冲区
    glfwSwapBuffers(window);
        // 处理事件
    glfwPollEvents();
}

void DisplayManager::CloseDisplay()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool DisplayManager::ShouldClose()
{
    return window ? glfwWindowShouldClose(window) : true;
}

int DisplayManager::getWidth() const
{
    return width;
}

int DisplayManager::getHeight() const
{
    return height;
}

double DisplayManager::getFrameTimeSeconds() const
{
    return FrameTime;
}
