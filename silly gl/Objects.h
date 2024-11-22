// Objects.h
#ifndef OBJECTS_H
#define OBJECTS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_SSE42 // or GLM_FORCE_SSE42 if your processor supports it
#define GLM_FORCE_ALIGNED
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <functional>
#include <algorithm> // Include this for std::remove

#include <useful.h>

// Has to be a global variable, as it is accessed in both classes
bool objectsUpdated = true;

// -------------------------------------------
// Declaration of GameObject class
class GameObject {
public:
	glm::vec3 position;
	glm::vec3 rotation;
	std::vector<glm::vec3> vertices;
	std::string name;
	std::vector<unsigned int> indices;

	GameObject(glm::vec3 pos, std::string handle)
		: position(pos), name(handle), rotation(glm::vec3(0.0f, 0.0f, 0.0f)), vertices({}), indices({}) {};

	void move(glm::vec3 change) {
		position += change;
		for (auto& vert : vertices) {
			vert += change;
		}
		objectsUpdated = true;
	}

	//faster operation for rotating vertices of an object using simd
	void rotate(glm::mat4 rotationMatrix) {
		for (auto& vert : vertices) {
			glm::vec3 translated = vert - position;
			glm::vec4 simd_translated = glm::vec4(translated, 1.0f);
			glm::vec4 simd_result = rotationMatrix * simd_translated;
			vert = glm::vec3(simd_result) + position;
		}
	}


    void scaleInPlace(glm::vec3 scale) {
		for (auto& vert : vertices) {
			vert -= position;
			vert *= scale;
			vert += position;
		}
		objectsUpdated = true;
    }

	void scale(glm::vec3 scale) {
		position *= scale;
		for (auto& vert : vertices) {
			vert *= scale;
		}
		objectsUpdated = true;
	}

	// Function to get the AABB of the GameObject
	// Given a min and max vector, it will set the min and max of the AABB
	void getAABB(glm::vec3& min, glm::vec3& max) const {
		if (vertices.empty()) {
			min = max = position;
			return;
		}
		min = max = vertices[0];
		for (const auto& vert : vertices) {
			min = glm::min(min, vert);
			max = glm::max(max, vert);
		}
	}
};

// Declaration of ObjectManager class
class ObjectManager {
public:
	ObjectManager() {
		GameObject* origin = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), "origin");
		origin->vertices = {glm::vec3(0.0f, 0.0f, 0.0f)};
		origin->indices = {0};
		objects.push_back(origin);
	}

	void setObjectsUpdated(bool updated) {
		objectsUpdated = updated;
	}

	void addObject(GameObject* object) {
		objects.push_back(object);
		objectsUpdated = true;
	}

	void destroyObject(GameObject* object) {
		objects.erase(std::remove(objects.begin(), objects.end(), object), objects.end());
		objectsUpdated = true;
	}

	bool checkCollision(GameObject* object1, GameObject* object2) {
		glm::vec3 min1, max1, min2, max2;
		object1->getAABB(min1, max1);
		object2->getAABB(min2, max2);

		// Check for overlap in all three axes
		bool collisionX = (min1.x <= max2.x && max1.x >= min2.x);
		bool collisionY = (min1.y <= max2.y && max1.y >= min2.y);
		bool collisionZ = (min1.z <= max2.z && max1.z >= min2.z);

		return collisionX && collisionY && collisionZ;
	}
	// Gets object by its handle name
	// If multiple objects have the same name, it only returns the first one to be created
	GameObject* getObjectByName(std::string name) {
		for (auto& object : objects) {
			if (object->name == name) {
				return object;
			}
		}
		return nullptr;
	}

	// Gets all objects with the given name
	std::vector<GameObject*> getObjectListByName(std::string name) {
		std::vector<GameObject*> objectsWithName = {};
		for (auto& object : objects) {
			if (object->name == name) {
				objectsWithName.push_back(object);
			}
		}
		return objectsWithName;
	}

	// Rotation is calculated once and stored locally in a variable until function is called again with a new rotation
	// Slightly faster for constant repeated rotations
	void rotateObjectsR(std::vector<GameObject*> objects, glm::vec3 rotation) {
		// Precompute the rotation matrix
		if(rotation != storedRotation)
		{
			glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
			glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
			storedRMatrix = rotationZ * rotationY * rotationX;
		}

		for (auto& object : objects) {
			object->rotate(storedRMatrix);
		}
		objectsUpdated = true;
	}

	bool haveObjectsUpdated() {
		if (objectsUpdated) {
			objectsUpdated = false;
			return true;
		}
		return false;
	}

	std::vector<GameObject*>* getObjects() {
		return &objects;
	}

	void addCube(float width, float height, float depth, glm::vec3 bottomLeft, std::string name) {
		std::vector<glm::vec3> vertices;
		std::vector<unsigned int> indices;

		glm::vec3 v0 = bottomLeft;
		glm::vec3 v1 = bottomLeft + glm::vec3(width, 0.0f, 0.0f);
		glm::vec3 v2 = bottomLeft + glm::vec3(width, height, 0.0f);
		glm::vec3 v3 = bottomLeft + glm::vec3(0.0f, height, 0.0f);
		glm::vec3 v4 = bottomLeft + glm::vec3(0.0f, 0.0f, depth);
		glm::vec3 v5 = bottomLeft + glm::vec3(width, 0.0f, depth);
		glm::vec3 v6 = bottomLeft + glm::vec3(width, height, depth);
		glm::vec3 v7 = bottomLeft + glm::vec3(0.0f, height, depth);

		vertices.insert(vertices.end(), { v0, v1, v2, v3, v4, v5, v6, v7 });

		indices.insert(indices.end(), {
			0, 1, 2, 2, 3, 0, // Front face
			4, 5, 6, 6, 7, 4, // Back face
			0, 3, 7, 7, 4, 0, // Left face
			1, 2, 6, 6, 5, 1, // Right face
			3, 2, 6, 6, 7, 3, // Top face
			0, 1, 5, 5, 4, 0  // Bottom face
			});

		glm::vec3 centre = bottomLeft + glm::vec3(width / 2, height / 2, depth / 2);

		GameObject* cube = new GameObject(centre, name);
		cube->vertices = vertices;
		cube->indices = indices;
		addObject(cube);
	}

private:
	std::vector<GameObject*> objects;
	glm::vec3 storedRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 storedRMatrix = glm::mat4(1.0f);
};
#endif // OBJECTS_H
