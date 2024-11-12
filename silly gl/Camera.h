#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	glm::vec3 position; // camera position as a vertex in the space
	glm::vec3 direction; // camera facing direction

	// Relative positionings

	glm::vec3 cameraFront; // Position always in front
	glm::vec3 cameraRight; // Position always to the right
	glm::vec3 cameraUp; // Position always above

	glm::mat4 view = glm::mat4(1.0f);
	Camera() {
		position = glm::vec3(0.0f, 0.0f, -3.0f);
		cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
		glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
		direction = glm::normalize(position - target);
		glm::vec3 true_up = glm::vec3(0.0f, 1.0f, 0.0f);
		cameraRight = glm::normalize(glm::cross(true_up, direction));
		cameraUp = glm::cross(direction, cameraRight);

		view = glm::lookAt(position, position + cameraFront, cameraUp);
	}
	void lookAt(glm::vec3 target) {
		cameraFront += target; // VERY WEIRD CODE, look at should face a position, and change direction facing, reliance on looking at a target should be removed to be honest...
		view = glm::lookAt(position, position + cameraFront, cameraUp);
	}
	void move(glm::vec3 movement) {
		position += movement;
		//Only allows movement in one direction. Movement should be based on DIRECTION FACING, and MAGNITUDE OF MOVEMENT IN VECTOR...
		view = glm::lookAt(position, position + cameraFront, cameraUp);
	}


};

