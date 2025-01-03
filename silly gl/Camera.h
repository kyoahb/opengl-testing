#ifndef CAMERA_H
#define CAMERA_H

#define _USE_MATH_DEFINES
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include "Useful.h"



class Camera
{
public:
	glm::vec3 position; // camera position as a vertex in the space
	glm::vec3 direction; // Pitch, -Yaw, Roll

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
	void changeDirection(glm::vec3 changeDirection) {
		// Pitch, Yaw, Roll -> changeDirection
		// Yaw is flipped, think OpenGL just does that
		direction += glm::vec3(changeDirection.x, -1*changeDirection.y, changeDirection.z);

		// Clamp the pitch to avoid flipping
		if (direction.x > 85.0f) {
			direction.x = 85.0f;
		}
		if (direction.x < -85.0f) {
			direction.x = -85.0f;
		}
		direction = vec3Clamp(direction, 360.0f);
		cameraFront = vec3Rotate(direction, glm::vec3(0.0f, 0.0f, 1.0f));
		view = glm::lookAt(position, position + cameraFront, cameraUp);
	}

	void move(std::string way) {
		float movementSpeed = 0.1f;

		glm::vec3 change = glm::vec3(0.0f, 0.0f, 0.0f);
		if (way == "forward") {
			change.z += (float)cos(direction.y * M_PI / 180);
			change.x += (float)sin(direction.y * M_PI / 180);
			change.y += (float)sin(-1 * direction.x * M_PI / 180);
		}
		else if (way == "back") {
			change.z -= (float)cos(direction.y * M_PI / 180);
			change.x -= (float)sin(direction.y * M_PI / 180);
			change.y -= (float)sin(-1 * direction.x * M_PI / 180);
		}
		else if (way == "left") {
			change.x += (float)cos(-1 * direction.y * M_PI / 180);
			change.z += (float)sin(-1 * direction.y * M_PI / 180);
		}
		else if (way == "right") {
			change.x -= (float)cos(-1 * direction.y * M_PI / 180);
			change.z -= (float)sin(-1 * direction.y * M_PI / 180);
		}
		position += change * movementSpeed;
		
		view = glm::lookAt(position, position + cameraFront, cameraUp);
	}

private:
	glm::vec3 cameraFront; // Position always in front
	glm::vec3 cameraRight; // Position always to the right
	glm::vec3 cameraUp; // Position always above
};
#endif

