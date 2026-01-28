#pragma once
#include <glad/glad.h>

class ModelTexture {
public:
    ModelTexture(GLuint texture);
    
    GLuint getID() const;

    int getNumberOfRows() const;
    float getShineDamper() const;
    float getReflectivity() const;
    bool isHasTransparency() const;
    bool isUseFakeLighting() const;
    int getNormalMap() const;
    bool isHasSpecularMap() const;
    int getSpecularMap() const;

    void setNumberOfRows(int n);
    void setShineDamper(float damper);
    void setReflectivity(float reflectivity);
    void setHasTransparency(bool hasTransparency);
    void setUseFakeLighting(bool useFakeLighting);
    void setNormalMap(int n);
    void setSpecularMap(int specMap);

    void setEnvironmentMap(int cubeMap);
    int getEnvironmentMap() const;
    bool isHasEnvironmentMap() const;
    
    void setReflectionStrength(float strength);
    float getReflectionStrength() const;
private:
    GLuint textureID;
    int normalMap;
    int specularMap;
    float shineDamper = 1;
    float reflectivity = 0;
    bool  hasTransparency = false;
    bool useFakeLighting = false;
    bool hasSpecularMap = false;
    int numberOfRows = 1;

    int environmentMap;
    float reflectionStrength = 0.5f;
    bool hasEnvironmentMap = false;
};
