#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <functional>
#include <Camera.h>
#include <Objects.h>

class Key {
public:
    const int keyCode;
	bool isHeld = false;
	std::function<void()> pressFunction = []() {};
	std::function<void()> holdFunction = []() {};

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
    InputManager(Camera* camera, ObjectManager* objManager) : globalCamera(camera), objectManager(objManager) {
		if (globalCamera == nullptr) {
			std::cerr << "Error: globalCamera is null" << std::endl;
		}
        createKey(GLFW_KEY_W, [this]() {}, [this]() {  globalCamera->move("forward");  });
		createKey(GLFW_KEY_A, [this]() {}, [this]() {  globalCamera->move("left");  });
		createKey(GLFW_KEY_S, [this]() {}, [this]() {  globalCamera->move("back");  });
		createKey(GLFW_KEY_D, [this]() {}, [this]() {  globalCamera->move("right");  });
		createKey(GLFW_KEY_E, [this]() {}, [this]() { objectManager->addCube(0.5f, 0.5f, 0.5f, glm::vec3(rand_float(-5, 5), rand_float(-5, 5), rand_float(-5, 5)), "cube"); });
		createKey(GLFW_KEY_Q, [this]() {}, [this]() {  
			objectManager->rotateObjectsR(objectManager->getObjectListByName("cube"), glm::vec3(10.0f, 0.0f, 0.0f));
			});

    }

	void createKey(int keyCode, std::function<void()> pressFunction, std::function<void()> holdFunction) {
		keys.emplace_back(Key(keyCode, pressFunction, holdFunction));
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

	// mouse linked to deltaTime
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

		float sensitivity = 0.05f;
		xoffset *= sensitivity * deltaTime;
		yoffset *= sensitivity * deltaTime;

		globalCamera->changeDirection(glm::vec3(yoffset, xoffset, 0.0f));
	};

	void update(double dTime) {
		deltaTime = dTime;
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
	ObjectManager* objectManager;
	std::vector<Key> keys;
	double deltaTime;
};