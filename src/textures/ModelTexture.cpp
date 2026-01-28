#include "ModelTexture.h"


ModelTexture::ModelTexture(GLuint texture) : textureID(texture)
{
}

// Getter实现
unsigned int ModelTexture::getID() const {
    return textureID;
}

int ModelTexture::getNumberOfRows() const
{
    return numberOfRows;
}

float ModelTexture::getShineDamper() const
{
    return shineDamper;
}

float ModelTexture::getReflectivity() const {
    return reflectivity;
}

bool ModelTexture::isHasTransparency() const
{
    return hasTransparency;
}

bool ModelTexture::isUseFakeLighting() const
{
    return useFakeLighting;
}

int ModelTexture::getNormalMap() const
{
    return normalMap;
}

bool ModelTexture::isHasSpecularMap() const
{
    return hasSpecularMap;
}

int ModelTexture::getSpecularMap() const
{
    return specularMap;
}

void ModelTexture::setNumberOfRows(int n)
{
    numberOfRows =  n;
}

// Setter实现
void ModelTexture::setShineDamper(float damper) {
    shineDamper = damper;
}

void ModelTexture::setReflectivity(float reflectivity) {
    this->reflectivity = reflectivity;
}

void ModelTexture::setHasTransparency(bool hasTransparency)
{
    this->hasTransparency = hasTransparency;
}

void ModelTexture::setUseFakeLighting(bool useFakeLighting)
{
    this -> useFakeLighting = useFakeLighting;
}

void ModelTexture::setNormalMap(int n)
{
    normalMap = n;
}

void ModelTexture::setSpecularMap(int specMap)
{
    specularMap = specMap;
    hasSpecularMap = true;
}

void ModelTexture::setEnvironmentMap(int cubeMap)
{
    environmentMap = cubeMap;
    hasEnvironmentMap = true;
}

int ModelTexture::getEnvironmentMap() const
{
    return environmentMap;
}

bool ModelTexture::isHasEnvironmentMap() const
{
    return hasEnvironmentMap;
}

void ModelTexture::setReflectionStrength(float strength)
{
    reflectionStrength = strength;
}

float ModelTexture::getReflectionStrength() const
{
    return reflectionStrength;
}
