#include "Maths.h"
#include "Camera.h"

float Maths::barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos)
{   
    float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
    float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y -p3.z)) / det;
    float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y -p3.z)) / det;
    float l3 = 1.0f - l1 -l2;
    return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}

glm::mat4 Maths::createTransformationMatrix(const glm::vec3 &translation, float rx, float ry, float rz, float scale)
{
    glm::mat4 matrix(1.0f);
    matrix = glm::translate(matrix, translation);
    matrix = glm::rotate(matrix, glm::radians(rx), glm::vec3(1.0f, 0.0f, 0.0f));
    matrix = glm::rotate(matrix, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
    matrix = glm::rotate(matrix, glm::radians(rz), glm::vec3(0.0f, 0.0f, 1.0f));
    matrix = glm::scale(matrix, glm::vec3(scale));
    return matrix;
}

glm::mat4 Maths::createViewMatrix(const Camera& camera) {
    glm::mat4 viewMatrix(1.0f);
    viewMatrix = glm::rotate(viewMatrix, glm::radians(camera.getPitch()), glm::vec3(1.0f, 0.0f, 0.0f));
    viewMatrix = glm::rotate(viewMatrix, glm::radians(camera.getYaw()), glm::vec3(0.0f, 1.0f, 0.0f));
    viewMatrix = glm::translate(viewMatrix, -camera.getPosition());
    return viewMatrix;
}

glm::mat4 Maths::createTransformationMatrix(const glm::vec2& translation, const glm::vec2& scale)
{
    glm::mat4 matrix(1.0f) ;
    matrix = glm::translate(matrix, glm::vec3(translation, 0.0f));
    matrix = glm::scale(matrix, glm::vec3(scale, 1.0f));

    return matrix;
}
