#include "MousePicker.h"


MousePicker::MousePicker(Camera& camera, const glm::mat4& projectionMatrix,Terrain& terrain)
    : camera(camera), projectionMatrix(projectionMatrix),terrain(terrain){
    viewMatrix = Maths::createViewMatrix(camera);
}

glm::vec3 MousePicker::getCurrentTerrainPoint() const {
    return currentTerrainPoint;
}

glm::vec3 MousePicker::getCurrentRay() const {
    return currentRay;
}

void MousePicker::update() {
    viewMatrix = Maths::createViewMatrix(camera);
    currentRay = calculateMouseRay();

    if (intersectionInRange(0, RAY_RANGE, currentRay)) {
        currentTerrainPoint = binarySearch(0, 0, RAY_RANGE, currentRay);

    } else {
        currentTerrainPoint = glm::vec3(0); // 默认值
    }
}

glm::vec3 MousePicker::calculateMouseRay() {
    double mouseX, mouseY;
    glfwGetCursorPos(glfwGetCurrentContext(), &mouseX, &mouseY);
    
    glm::vec2 normalizedCoords = getNormalisedDeviceCoordinates(
        static_cast<float>(mouseX), 
        static_cast<float>(mouseY));
    //std::cout << "NDC: (" << normalizedCoords.x << ", " << normalizedCoords.y << ")" << std::endl;
    glm::vec4 clipCoords(normalizedCoords.x, normalizedCoords.y, -1.0f, 1.0f);
    glm::vec4 eyeCoords = toEyeCoords(clipCoords);
    return toWorldCoords(eyeCoords);
}

glm::vec3 MousePicker::toWorldCoords(const glm::vec4& eyeCoords) {
    glm::mat4 invertedView = glm::inverse(viewMatrix);
    glm::vec4 rayWorld = invertedView * eyeCoords;
    return glm::normalize(glm::vec3(rayWorld.x,rayWorld.y,rayWorld.z));
}

glm::vec4 MousePicker::toEyeCoords(const glm::vec4& clipCoords) {
    glm::mat4 invertedProjection = glm::inverse(projectionMatrix);
    glm::vec4 eyeCoords = invertedProjection * clipCoords;
    return glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);
}

glm::vec2 MousePicker::getNormalisedDeviceCoordinates(float mouseX, float mouseY) {
    int width, height;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
    
    float x = (2.0f * mouseX) / width - 1.0f;
    float y = 1.0f -(2.0f * mouseY) / height ;  // GLFW的y坐标是从上往下的
    return glm::vec2(x, y);
}

glm::vec3 MousePicker::getPointOnRay(const glm::vec3& ray, float distance) {
    glm::vec3 camPos = camera.getPosition();
    glm::vec3 scaledRay = ray * distance;
    return camPos + scaledRay;
}

glm::vec3 MousePicker::binarySearch(int count, float start, float finish, const glm::vec3& ray) {
    float half = start + ((finish - start) / 2.0f);
    if (count >= RECURSION_COUNT) {
        glm::vec3 endPoint = getPointOnRay(ray, half);
        auto terrain = getTerrain(endPoint.x, endPoint.z);
        if (terrain) {
            return endPoint;
        }
        return glm::vec3(0);
    }
    
    if (intersectionInRange(start, half, ray)) {
        return binarySearch(count + 1, start, half, ray);
    } else {
        return binarySearch(count + 1, half, finish, ray);
    }
}

bool MousePicker::intersectionInRange(float start, float finish, const glm::vec3& ray) {
    glm::vec3 startPoint = getPointOnRay(ray, start);
    glm::vec3 endPoint = getPointOnRay(ray, finish);
    float startHeight = terrain.getHeightOfTerrain(startPoint.x, startPoint.z);
    float endHeight = terrain.getHeightOfTerrain(endPoint.x, endPoint.z);
    //std::cout << "Start point height: " << startHeight << ", End point height: " << endHeight << std::endl;
    return !isUnderGround(startPoint) && isUnderGround(endPoint);
}

bool MousePicker::isUnderGround(const glm::vec3& testPoint) {
    auto terrain = getTerrain(testPoint.x, testPoint.z);
    if (terrain!= nullptr) {
        float height = terrain->getHeightOfTerrain(testPoint.x, testPoint.z);
        return testPoint.y < height;
    }
    return false;
}

Terrain* MousePicker::getTerrain(float worldX, float worldZ) {
    float terrainX = terrain.getX();
    float terrainZ = terrain.getZ();
    float terrainSize = terrain.getSize(); 

    if (worldX >= terrainX && worldX < terrainX + terrainSize &&
        worldZ >= terrainZ && worldZ < terrainZ + terrainSize) {
        return &terrain;
    }
    return nullptr; 
}
