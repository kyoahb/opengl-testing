#define _USE_MATH_DEFINES
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <useful.h>
#include <math.h>



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
	void changeDirection(glm::vec3 changeDirection) {
		//something wrong with the Y values here...
		direction += changeDirection;
		direction = vec3Clamp(direction, 360.0f);
		cameraFront = vec3Rotate(changeDirection, cameraFront);
		outputInfo();
		view = glm::lookAt(position, position + cameraFront, cameraUp);
	}

	void move(std::string way) {
		float movementSpeed = 0.1f;

		glm::vec3 change = glm::vec3(0.0f, 0.0f, 0.0f);
		if (way == "forward") {
			change.z += (float)cos(direction.x * M_PI / 180);
			change.x += (float)sin(direction.x * M_PI / 180);
			change.y += (float)sin(direction.y * M_PI / 180);
		}
		else if (way == "back") {
			change.z -= (float)cos(direction.x * M_PI / 180);
			change.x -= (float)sin(direction.x * M_PI / 180);
			change.y -= (float)sin(direction.y * M_PI / 180);
		}
		else if (way == "left") {
			change.x += (float)cos(-1*direction.x * M_PI / 180);
			change.z += (float)sin(-1*direction.x * M_PI / 180);
		}
		else if (way == "right") {
			change.x -= (float)cos(-1*direction.x * M_PI / 180);
			change.z -= (float)sin(-1*direction.x * M_PI / 180);
		}

		vec3Print(position, "position pre-move");
		position += change * movementSpeed;
		vec3Print(position, "position post-move");
		
		view = glm::lookAt(position, position + cameraFront, cameraUp);
	}
	
	void outputInfo() {
		vec3Print(cameraFront, "cameraFront");
		vec3Print(direction, "cameraDirection");
		vec3Print(position, "cameraPosition");
	}

};


