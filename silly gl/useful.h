#ifndef USEFUL_H
#define USEFUL_H

#define GLM_FORCE_SSE42 // or GLM_FORCE_SSE42 if your processor supports it
#define GLM_FORCE_ALIGNED

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

//Rotates a vector by a given rotation about the origin
glm::vec3 vec3Rotate(glm::vec3 rotation, glm::vec3 original) {
    glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    // Combine rotations
    glm::mat4 rotationMatrix = rotationZ * rotationY * rotationX;

    original = rotationMatrix * glm::vec4(original, 0.0f);
    original = glm::vec3(original);
    return original;
}

//Rotates a vector by a given rotation about a given point
glm::vec3 vec3RotateAroundPoint(glm::vec3 rotation, glm::vec3 point, glm::vec3 original) {
    // Translate the point to the origin
    glm::vec3 translated = original - point;

    // Perform the rotation
    glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    // Combine rotations
    glm::mat4 rotationMatrix = rotationZ * rotationY * rotationX;

    translated = rotationMatrix * glm::vec4(translated, 0.0f);
    translated = glm::vec3(translated);

    // Translate the point back
    glm::vec3 result = translated + point;

    return result;
}

glm::vec3 vec3Clamp(glm::vec3 vector, float clamp) {
    return glm::vec3(fmodf(vector.x, clamp), fmodf(vector.y, clamp), fmodf(vector.z, clamp));
}

void vec3Print(glm::vec3 vector, std::string name) {
    std::cout << name << " X IS" << vector.x << " AND Y IS " << vector.y << " AND Z IS " << vector.z << std::endl;
}

float rand_float(float a, float b)
{
    if (a > b) std::swap(a, b);
    return a + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (b - a)));
}
#endif