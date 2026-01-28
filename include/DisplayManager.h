#pragma once 
#include<glad/glad.h>
#include<GL/gl.h>
#include<GLFW/glfw3.h>
#include<iostream>
class DisplayManager{

public:
    static DisplayManager& getInstance();
    
    bool CreateDisplay();
    void UpdateDisplay();
    void CloseDisplay();
    bool ShouldClose();
    int  getWidth() const;
    int  getHeight() const; 
    double getFrameTimeSeconds() const;
private:
    DisplayManager();

    DisplayManager(const DisplayManager&) = delete;
    DisplayManager& operator=(const DisplayManager&) = delete;

    
private:
    static constexpr int width  = 1280;
    static constexpr int height = 720; 
    static constexpr int FPS_CAP = 60;
    
    const char* Title = "Our First Display";
    GLFWwindow* window;
    double LastFrameTime ;
    double FrameTime;
};
