// Objects.h
#ifndef OBJECTS_H
#define OBJECTS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
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

	GameObject(std::vector<glm::vec3> verts, glm::vec3 pos, std::string handle)
		: vertices(verts), position(pos), name(handle), rotation(glm::vec3(0.0f, 0.0f, 0.0f)) {};

	void move(glm::vec3 change) {
		position += change;
		for (auto& vert : vertices) {
			vert += change;
		}
		objectsUpdated = true;
	}

	void rotate(glm::vec3 rot) {
		for (auto& vert : vertices) {
			vert = vec3RotateAroundPoint(rot, position, vert);
		}
		objectsUpdated = true;
	}

	void scale(glm::vec3 scale) {
		position = scale * position;
		for (auto& vert : vertices) {
			vert = scale * vert;
		}
		objectsUpdated = true;
	}
};

// Declaration of ObjectManager class
class ObjectManager {
public:
	ObjectManager() {
		objects.push_back(new GameObject({ glm::vec3(0.0f, 0.0f, 0.0f) }, glm::vec3(0.0f, 0.0f, 0.0f), "origin"));
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

	void rotateMultipleObjects(std::vector<GameObject*> objects, glm::vec3 rotation) {
		for (auto& object : objects) {
			object->rotate(rotation);
		}
	}

	bool haveObjectsUpdated() {
		if (objectsUpdated) {
			objectsUpdated = false;
			return true;
		}
		return false;
	}

	std::vector<std::vector<glm::vec3>> getAllVertices() {
		std::vector<std::vector<glm::vec3>> vertices;
		for (auto& object : objects) {
			vertices.push_back(object->vertices);
		}
		return vertices;
	}

	void addCube(float width, float height, float depth, glm::vec3 bottomLeft, std::string name) {
		std::vector<glm::vec3> vertices;

		glm::vec3 v0 = bottomLeft;
		glm::vec3 v1 = bottomLeft + glm::vec3(width, 0.0f, 0.0f);
		glm::vec3 v2 = bottomLeft + glm::vec3(width, height, 0.0f);
		glm::vec3 v3 = bottomLeft + glm::vec3(0.0f, height, 0.0f);
		glm::vec3 v4 = bottomLeft + glm::vec3(0.0f, 0.0f, depth);
		glm::vec3 v5 = bottomLeft + glm::vec3(width, 0.0f, depth);
		glm::vec3 v6 = bottomLeft + glm::vec3(width, height, depth);
		glm::vec3 v7 = bottomLeft + glm::vec3(0.0f, height, depth);

		vertices.insert(vertices.end(), { v0, v1, v2, v2, v3, v0 });
		vertices.insert(vertices.end(), { v4, v5, v6, v6, v7, v4 });
		vertices.insert(vertices.end(), { v0, v3, v7, v7, v4, v0 });
		vertices.insert(vertices.end(), { v1, v2, v6, v6, v5, v1 });
		vertices.insert(vertices.end(), { v3, v2, v6, v6, v7, v3 });
		vertices.insert(vertices.end(), { v0, v1, v5, v5, v4, v0 });

		glm::vec3 centre = bottomLeft + glm::vec3(width/2, height/2, depth/2);

		GameObject* cube = new GameObject(vertices, centre, name);
		addObject(cube);
	}

private:
	std::vector<GameObject*> objects;
};
#endif // OBJECTS_H
