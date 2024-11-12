#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

glm::vec3 vec3Rotate(glm::vec3 rotation, glm::vec3 original) {
	glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(-1.0f, 0.0f, 0.0f));
	glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	// Combine rotations
	glm::mat4 rotationMatrix = rotationX * rotationY * rotationZ;

	original = rotationMatrix * glm::vec4(original, 0.0f);
	original = glm::vec3(original);
	return original;
}

glm::vec3 vec3Clamp(glm::vec3 vector, float clamp) {
	return glm::vec3(fmodf(vector.x, clamp), fmodf(vector.y, clamp), fmodf(vector.z, clamp));
}

void vec3Print(glm::vec3 vector, std::string name) {
	std::cout << name << " X IS" << vector.x << " AND Y IS " << vector.y << " AND Z IS " << vector.z << std::endl;
}