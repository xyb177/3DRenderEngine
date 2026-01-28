#include "Vertex.h"

Vertex::Vertex(int index, const glm::vec3& position) 
    : index(index), position(position), length(glm::length(position)) {}

void Vertex::addTangent(const glm::vec3& tangent) {
    tangents.push_back(tangent);
}

void Vertex::averageTangents() {
    if(tangents.empty()) return;
    
    for(const auto& tangent : tangents) {
        averagedTangent += tangent;
    }
    averagedTangent = glm::normalize(averagedTangent);
}

glm::vec3 Vertex::getAverageTangent() const {
    return averagedTangent;
}

    
int Vertex::getIndex() const { 
    return index; 
}

float Vertex::getLength() const { 
    return length; 
}

bool Vertex::isSet() const {
    return textureIndex != NO_INDEX && normalIndex != NO_INDEX;
}

bool Vertex::hasSameTextureAndNormal(int textureIndexOther, int normalIndexOther) const {
    return textureIndexOther == textureIndex && normalIndexOther == normalIndex;
}

void Vertex::setTextureIndex(int textureIndex) {
    this->textureIndex = textureIndex;
}

void Vertex::setNormalIndex(int normalIndex) {
    this->normalIndex = normalIndex;
}

const glm::vec3& Vertex::getPosition() const {
    return position; 
}

int Vertex::getTextureIndex() const { 
    return textureIndex; 
}

int Vertex::getNormalIndex() const { 
    return normalIndex; 
}

Vertex* Vertex::getDuplicateVertex() const { 
    return duplicateVertex; 
}

void Vertex::setDuplicateVertex(Vertex* duplicateVertex) {
    this->duplicateVertex = duplicateVertex;
}
