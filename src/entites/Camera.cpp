#include "Camera.h"

Camera::Camera(Player& player):player(player),window(glfwGetCurrentContext()) {
    glfwSetScrollCallback(window, scrollCallback);
}

void Camera::move() {
   calculateZoom();
   calculatePitch();
   calculateAngleAroundPlayer();
   float horizontalDistance = calculateHorizontalDistance();
   float verticalDistance = calculateVerticalDistance();
   calculateCameraPosition(horizontalDistance,verticalDistance);
   yaw = 180 - (player.getRotY() + angleAroundPlayer);
}

const glm::vec3& Camera::getPosition() const { return position; }
float Camera::getPitch() const { return pitch; }
float Camera::getYaw() const { return yaw; }
float Camera::getRoll() const { return roll; }

void Camera::setSpeed(float n)
{
    moveSpeed = n;
}

void Camera::setPosition(float x, float y, float z)
{
    this->position = glm::vec3(x,y,z);
}

void Camera::invertPitch()
{
    this->pitch = -pitch; 
}

void Camera::scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    scrollY += yoffset;
}

void Camera::calculateZoom()
{
    distanceFromPlayer -= static_cast<float>(scrollY) * 0.5f;
    distanceFromPlayer = glm::clamp(distanceFromPlayer, 5.0f, 1000.0f);
    scrollY = 0;
}

void Camera::calculatePitch()
{
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        double currentY;
        glfwGetCursorPos(window, nullptr, &currentY);
        
        if (firstMousePressPitch) {
            lastMouseY = currentY;
            firstMousePressPitch = false;
        }

        float pitchChange = static_cast<float>(currentY - lastMouseY) * 0.1f;
        pitch -= pitchChange;
        lastMouseY = currentY;
    } else {
        firstMousePressPitch = true;  // 松开鼠标后重置
    }
}

void Camera::calculateAngleAroundPlayer()
{
   if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        double currentX;
        glfwGetCursorPos(window, &currentX, nullptr);
        
        if (firstMousePressYaw) {
            lastMouseX = currentX;
            firstMousePressYaw = false;
        }

        float angleChange = static_cast<float>(currentX - lastMouseX) * 0.1f;
        angleAroundPlayer -= angleChange;
        lastMouseX = currentX;
    } else {
        firstMousePressYaw = true;  // 松开鼠标后重置
    }
}

float Camera::calculateHorizontalDistance()
{
    return static_cast<float>(distanceFromPlayer * std::cos(pitch*(M_PI /180.0f)));
}

float Camera::calculateVerticalDistance()
{
    return static_cast<float>(distanceFromPlayer * std::sin(pitch*(M_PI /180.0f)));
}

void Camera::calculateCameraPosition(float horizDistance, float verticDistance)
{   float theta = player.getRotY() + angleAroundPlayer;
    float offsetX = horizDistance * std::sin(theta * (M_PI/180.0f));
    float offsetZ = horizDistance * std::cos(theta * (M_PI/180.0f));
    position.x = player.getPosition().x - offsetX;
    position.z = player.getPosition().z - offsetZ;
    position.y = player.getPosition().y + verticDistance;
}
