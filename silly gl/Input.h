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
	std::function<void()> pressFunction;
	std::function<void()> holdFunction;

	Key(int keyCode)
		: keyCode(keyCode), pressFunction([]() {}), holdFunction([]() {}) {}

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

		Key forward = Key(GLFW_KEY_W);
		forward.holdFunction = [this]() { globalCamera->move("forward"); };
		addKey(forward);

		Key left = Key(GLFW_KEY_A);
		left.holdFunction = [this]() { globalCamera->move("left"); };
		addKey(left);

		Key back = Key(GLFW_KEY_S);
		back.holdFunction = [this]() { globalCamera->move("back"); };
		addKey(back);

		Key right = Key(GLFW_KEY_D);
		right.holdFunction = [this]() { globalCamera->move("right"); };
		addKey(right);

		Key addCube = Key(GLFW_KEY_E);
		addCube.holdFunction = [this]() {
			objectManager->addCube(0.5f, 0.5f, 0.5f, glm::vec3(rand_float(-5, 5), rand_float(-5, 5), rand_float(-5, 5)), "cube");
			};
		addKey(addCube);

		Key rotateCubes = Key(GLFW_KEY_Q);
		rotateCubes.holdFunction = [this]() {
			objectManager->rotateObjectsR(objectManager->getObjectListByName("cube"), (float)deltaTime*glm::vec3(360.0f, 0.0f, 0.0f));
			};
		addKey(rotateCubes);

		Key scaleCubes = Key(GLFW_KEY_R);
		scaleCubes.pressFunction = [this]() {
			for (auto& object : objectManager->getObjectListByName("cube")) {
				object->scale(glm::vec3(2.0f, 2.0f, 2.0f));
			}
			};
		addKey(scaleCubes);

		Key scaleIPCubes = Key(GLFW_KEY_T);
		scaleIPCubes.pressFunction = [this]() {
			for (auto& object : objectManager->getObjectListByName("cube")) {
				object->scaleInPlace(glm::vec3(2.0f, 2.0f, 2.0f));
			}
			};
		addKey(scaleIPCubes);

	}

	void addKey(Key key) {
		keys.emplace_back(key);
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
		xoffset *= sensitivity;
		yoffset *= sensitivity;

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
	float timeSinceLastRotation = 0.0f;
};