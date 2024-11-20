// Renderer.h
#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>
#include <iostream>
#include <Camera.h>
#include <shader_l.h>
#include <Objects.h>

class Renderer {
public:
    Shader shader;
    std::vector<GLint> firsts;
    std::vector<GLsizei> counts;
    bool verticesUpdated = true;
    unsigned int SCR_WIDTH;
    unsigned int SCR_HEIGHT;
    std::vector<std::vector<glm::vec3>> objectVertices = { std::vector<glm::vec3>{glm::vec3(0.0f, 0.0f, 0.0f)} };
    Camera* globalCamera;
    ObjectManager* objectManager;

    Renderer(Camera* camera, ObjectManager* objManager, unsigned int scr_width, unsigned int scr_height) : 
        shader(Shader("shaders/shader.vs", "shaders/shader.fs")), 
        globalCamera(camera), 
        objectManager(objManager), 
        SCR_WIDTH(scr_width), 
        SCR_HEIGHT(scr_height) 
    {
        objectVertices = objectManager->getAllVertices();
        verticesUpdated = objectManager->haveObjectsUpdated();
    }

    void render() {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // render the triangle
        shader.use();

        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 view = globalCamera->view;
        glm::mat4 projection = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        projection = glm::perspective(glm::radians(60.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        verticesUpdated = objectManager->haveObjectsUpdated();
		// If vertices have been updated, update the buffer data for each object
        if (verticesUpdated) {
            objectVertices = objectManager->getAllVertices();
            std::vector<glm::vec3> combined_vertices = {};
            firsts = {};
            counts = {};

            GLsizei offset = 0;
            for (const auto& verts : objectVertices) {
                combined_vertices.insert(combined_vertices.end(), verts.begin(), verts.end());
                firsts.push_back(offset);
                counts.push_back(static_cast<GLsizei>(verts.size()));
                offset += static_cast<GLsizei>(verts.size());
            }

            glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(combined_vertices.size() * vecSize), combined_vertices.data(), GL_DYNAMIC_DRAW);
        }
        glMultiDrawArrays(GL_TRIANGLES, firsts.data(), counts.data(), static_cast<GLsizei>(counts.size()));
    }

private:
    float const vecSize = sizeof(float) * 3;
};
#endif