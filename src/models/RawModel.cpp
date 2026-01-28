#pragma once
#include "RawModel.h"

RawModel::RawModel(unsigned int vaoID, int vertexCount):VaoID(vaoID),VertexCount(vertexCount)
{
}

unsigned int RawModel::getVaoID() const
{
    return VaoID;
}

int RawModel::getVertexCount() const
{
    return VertexCount;
}
