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

class Renderer {
public:
    Shader shader;
    std::vector<GLint> firsts;
    std::vector<GLsizei> counts;
    bool verticesUpdated = true;
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;
    std::vector<std::vector<glm::vec3>> drawn_vertices = { std::vector<glm::vec3>{glm::vec3(0.0f, 0.0f, 0.0f)} };
    Camera* globalCamera;

    Renderer(Camera* camera) : shader(Shader("shaders/shader.vs", "shaders/shader.fs")), globalCamera(camera) {
    }

    void render() {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // render the triangle
        shader.use();

        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //view = glm::translate(view, globalCamera.position);
        view = globalCamera->view;
        projection = glm::perspective(glm::radians(60.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);


        if (verticesUpdated && !drawn_vertices.empty()) {
            std::vector<glm::vec3> combined_vertices = {};
            firsts = {};
            counts = {};

            GLsizei offset = 0;
            for (const auto& verts : drawn_vertices) {
                combined_vertices.insert(combined_vertices.end(), verts.begin(), verts.end());
                firsts.push_back(offset);
                counts.push_back(static_cast<GLsizei>(verts.size()));
                offset += verts.size();
            }

            glBufferData(GL_ARRAY_BUFFER, combined_vertices.size() * vecSize, combined_vertices.data(), GL_DYNAMIC_DRAW);
        }
        glMultiDrawArrays(GL_TRIANGLES, firsts.data(), counts.data(), static_cast<GLsizei>(counts.size()));
    }

    void draw_cube(float width, float height, float depth, glm::vec3 bottom_left) {
        // Define the six faces of the cube
        std::vector<std::vector<glm::vec3>> faces = {
            form_square(width, height, bottom_left + glm::vec3(0, 0, depth), glm::vec3(0, 0, 0)),        // Front face
            form_square(width, height, bottom_left + glm::vec3(width, 0, 0), glm::vec3(0, 180, 0)),      // Back face
            form_square(width, depth, bottom_left + glm::vec3(0, height, depth), glm::vec3(-90, 0, 0)),  // Top face
            form_square(width, depth, bottom_left, glm::vec3(90, 0, 0)),                                 // Bottom face
            form_square(depth, height, bottom_left + glm::vec3(width, 0, 0), glm::vec3(0, -90, 0)),      // Right face
            form_square(depth, height, bottom_left + glm::vec3(0, 0, depth), glm::vec3(0, 90, 0))        // Left face
        };

        // Draw each face
        for (const auto& face : faces) {
            draw_vertices(face);
        }
    }

    // Returns a vector of vertices that form a square with given width, height, and bottom_left coord
    std::vector<glm::vec3> form_square(float width, float height, glm::vec3 bottom_left, glm::vec3 angles) {
        // Create initial vertices of the square relative to (0, 0, 0)
        std::vector<glm::vec3> verts = {
            glm::vec3(0, 0, 0),                   // bottom left
            glm::vec3(width, 0, 0),               // bottom right
            glm::vec3(0, height, 0),              // top left
            glm::vec3(width, height, 0)           // top right
        };

        // Create rotation matrices for each axis
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angles.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(angles.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(angles.z), glm::vec3(0.0f, 0.0f, 1.0f));

        // Translate each vertex, rotate, and then move to bottom_left position
        for (auto& vert : verts) {
            vert = glm::vec3(rotationMatrix * glm::vec4(vert, 1.0f)) + bottom_left;
        }

        // Return vertices as triangles to form the square
        return {
            verts[0], verts[1], verts[2],  // First triangle
            verts[2], verts[1], verts[3]   // Second triangle
        };
    }

    // Adds a vector of vertices to the vertices list to be drawn next frame...
    void draw_vertices(std::vector<glm::vec3> verts) {
        verticesUpdated = true;
        drawn_vertices.push_back(verts);
    }

private:
    float const vecSize = sizeof(float) * 3;
};
#endif