#pragma once
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

class Camera;
class Maths{
public:

    static float barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos);

    static glm::mat4 createTransformationMatrix(const glm::vec3& translation,
                                              float rx, float ry, float rz,
                                              float scale);
    static glm::mat4 createTransformationMatrix(const glm::vec2& translation, const glm::vec2& scale);
    static glm::mat4 createViewMatrix(const Camera& camera);
  
};

