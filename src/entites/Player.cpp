#include "Player.h"

Player::Player(TexturedModel model, const glm::vec3& position, float rotX, float rotY, float rotZ, float scale)
    :Entity(model, position,rotX,rotY,rotZ,scale)
{

}

void Player::move(Terrain& terrain)
{
    checkInputs();
    double delta = DisplayManager::getInstance().getFrameTimeSeconds();
    this->increaseRotation(0,static_cast<float>(currentTurnSpeed * delta),0);
    double distance = currentSpeed * delta;
    float dx = static_cast<float>(distance * std::sin(this->getRotY()*(M_PI /180.0f)));
    float dz = static_cast<float>(distance * std::cos(this->getRotY()*(M_PI /180.0f)));
    this->increasePosition(dx,0,dz);
    upwardsSpeed += static_cast<float>(GRAVITY * delta);
    this->increasePosition(0,static_cast<float>(upwardsSpeed * delta),0);
    float terrainHeight = terrain.getHeightOfTerrain(this->getPosition().x,this->getPosition().z);
    if(this->getPosition().y < terrainHeight)
    {   
        upwardsSpeed = 0;
        isInAir = false;
        auto temp = this->getPosition();
        this->setPosition(glm::vec3(temp.x,terrainHeight,temp.z));
    }
}

void Player::jump()
{   if (!isInAir){
        upwardsSpeed = JUMP_POWER;
        isInAir = true;
    }
        
}

void Player::checkInputs()
{
    GLFWwindow* window = glfwGetCurrentContext(); 
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        currentSpeed = RUN_SPEED;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        currentSpeed = -RUN_SPEED;
    }
    else {
        currentSpeed = 0;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        currentTurnSpeed = -TURN_SPEED;
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        currentTurnSpeed = TURN_SPEED;
    }
    else {
        currentTurnSpeed = 0;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        jump();
    }  
   
}
