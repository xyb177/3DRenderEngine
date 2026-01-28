#pragma once
#include "DisplayManager.h"
#include "Entity.h"
#include <cmath>
#include "Terrain.h"
class Player : public Entity{

public:
    Player(TexturedModel model, 
           const glm::vec3& position, 
           float rotX, float rotY, float rotZ, 
           float scale);

    void move(Terrain& terrain);
    void jump();
private:
    void checkInputs();
private:
    static constexpr float RUN_SPEED = 20;
    static constexpr float TURN_SPEED = 160;
    static constexpr float GRAVITY = -50;
    static constexpr float JUMP_POWER = 30;
    static constexpr float TERRAIN_HEIGHT = 0;

    float currentSpeed = 0;
    float currentTurnSpeed = 0;
    float upwardsSpeed = 0;

    bool isInAir = false;
};           