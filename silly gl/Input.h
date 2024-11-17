#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <functional>
#include <Camera.h>
#include <Renderer.h>

class Key {
public:
    const int keyCode;
	bool isHeld = false;
	std::function<void()> pressFunction;
	std::function<void()> holdFunction;

	Key(int keyCode, std::function<void()> pressFunction, std::function<void()> holdFunction)
		: keyCode(keyCode), pressFunction(pressFunction), holdFunction(holdFunction) {}

	void press() {
		isHeld = true;
		if (pressFunction) {
			pressFunction();
		}
	}

	void release() {
		isHeld = false;
	}
	
	// hold functions are managed inside a hold function update within the input manager, run every frame.

};

class InputManager {
public:
    InputManager(Camera* camera) : globalCamera(camera) {
		if (globalCamera == nullptr) {
			std::cerr << "Error: globalCamera is null" << std::endl;
		}
        keys.emplace_back(Key(GLFW_KEY_W, [this]() {}, [this]() {  globalCamera->move("forward");  }));
		keys.emplace_back(Key(GLFW_KEY_E, [this]() {
			for (int i = 0; i <= 10; i++) {
				renderer->draw_cube(0.5, 0.5, 0.5, glm::vec3(rand_float(-5, 5), rand_float(-5, 5), rand_float(-5, 5)));
		}}, [this]() { /* hold function for key E */ }));
    }

	void setRenderer(Renderer* render) {
		renderer = render;
	}

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		for (auto& k : keys) {
			if (k.keyCode == key) {
				if (action == GLFW_PRESS) {
					k.press();
				}
				else if (action == GLFW_RELEASE) {
					k.release();
				}
				break;
			}
		}
	};

	void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
		static double lastX = xpos;
		static double lastY = ypos;
		static bool firstMouse = true;

		if (firstMouse) {
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		double xoffset = xpos - lastX;
		double yoffset = ypos - lastY;

		lastX = xpos;
		lastY = ypos;

		float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		globalCamera->changeDirection(glm::vec3(yoffset, xoffset, 0.0f));
	};

	void update(float deltaTime) {
		manageHeldKeys();
	}

	void manageHeldKeys() {
		for (auto& k : keys) {
			if (k.isHeld && k.holdFunction) {
				k.holdFunction();
			}
		}
	}

	Key* getKey(int keyCode) {
		for (auto k : keys) {
			if (k.keyCode == keyCode) {
				return &k;
			}
		}
		return nullptr; // Return nullptr if key not found
	}
private:
	Camera* globalCamera;
	Renderer* renderer;
	std::vector<Key> keys;
};