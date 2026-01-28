#pragma once
#include <glad/glad.h>
#include<GL/gl.h>
#include<GLFW/glfw3.h>
#include<iostream>

class RawModel
{
public:
    RawModel(unsigned int vaoID, int vertexCount);

    unsigned int getVaoID() const;
    int getVertexCount() const;
private:
    unsigned int VaoID;
    int VertexCount;

};