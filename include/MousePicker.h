#pragma once
#include "glm.hpp"
#include <memory>
#include "Camera.h"
#include "Terrain.h"
#include "Maths.h"
class MousePicker {
public:
    static constexpr int RECURSION_COUNT = 200;
    static constexpr float RAY_RANGE = 600.0f;

    MousePicker(Camera& camera, 
                const glm::mat4& projectionMatrix,
                Terrain& terrain);

    glm::vec3 getCurrentTerrainPoint() const;
    glm::vec3 getCurrentRay() const;
    void update();

private:
    glm::vec3 calculateMouseRay();
    glm::vec3 toWorldCoords(const glm::vec4& eyeCoords);
    glm::vec4 toEyeCoords(const glm::vec4& clipCoords);
    glm::vec2 getNormalisedDeviceCoordinates(float mouseX, float mouseY);

    // Ray casting methods
    glm::vec3 getPointOnRay(const glm::vec3& ray, float distance);
    glm::vec3 binarySearch(int count, float start, float finish, const glm::vec3& ray);
    bool intersectionInRange(float start, float finish, const glm::vec3& ray);
    bool isUnderGround(const glm::vec3& testPoint);
    Terrain* getTerrain(float worldX, float worldZ);

    glm::vec3 currentRay;
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    Camera& camera;
    Terrain& terrain;
    glm::vec3 currentTerrainPoint;
};
