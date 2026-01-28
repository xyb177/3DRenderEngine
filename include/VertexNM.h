#pragma once
#include <vector>
#include "glm.hpp"

class VertexNM {

public:
    VertexNM(int index, const glm::vec3& position);
    
    void addTangent(const glm::vec3& tangent);
    void averageTangents();
    glm::vec3 getAverageTangent() const;
    
    int getIndex() const;
    float getLength() const;
    bool isSet() const;
    bool hasSameTextureAndNormal(int textureIndexOther, int normalIndexOther) const;
    
    void setTextureIndex(int textureIndex);
    void setNormalIndex(int normalIndex);
    
    const glm::vec3& getPosition() const;
    int getTextureIndex() const;
    int getNormalIndex() const;
    
    VertexNM* getDuplicateVertex() const;
    void setDuplicateVertex(VertexNM* duplicateVertex);

private:
    static const int NO_INDEX = -1;
    
    glm::vec3 position;
    int textureIndex = NO_INDEX;
    int normalIndex = NO_INDEX;
    VertexNM* duplicateVertex = nullptr;
    int index;
    float length;
    std::vector<glm::vec3> tangents;
    glm::vec3 averagedTangent = glm::vec3(0, 0, 0);
};
