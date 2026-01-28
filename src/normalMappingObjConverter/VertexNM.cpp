#include "VertexNM.h"

VertexNM::VertexNM(int index, const glm::vec3& position)
    : index(index), position(position), length(glm::length(position)) {}

void VertexNM::addTangent(const glm::vec3& tangent) {
    tangents.push_back(tangent);
}

void VertexNM::averageTangents() {
    if(tangents.empty()) return;
    
    for(const auto& tangent : tangents) {
        averagedTangent += tangent;
    }
    averagedTangent = glm::normalize(averagedTangent);
}

glm::vec3 VertexNM::getAverageTangent() const {
    return averagedTangent;
}

int VertexNM::getIndex() const { return index; }
float VertexNM::getLength() const { return length; }
bool VertexNM::isSet() const { return textureIndex != NO_INDEX && normalIndex != NO_INDEX; }

bool VertexNM::hasSameTextureAndNormal(int textureIndexOther, int normalIndexOther) const {
    return textureIndexOther == textureIndex && normalIndexOther == normalIndex;
}

void VertexNM::setTextureIndex(int textureIndex) { this->textureIndex = textureIndex; }
void VertexNM::setNormalIndex(int normalIndex) { this->normalIndex = normalIndex; }

const glm::vec3& VertexNM::getPosition() const { return position; }
int VertexNM::getTextureIndex() const { return textureIndex; }
int VertexNM::getNormalIndex() const { return normalIndex; }

VertexNM* VertexNM::getDuplicateVertex() const { return duplicateVertex; }
void VertexNM::setDuplicateVertex(VertexNM* duplicateVertex) { 
    this->duplicateVertex = duplicateVertex; 
}
