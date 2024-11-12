#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <shader_l.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <Camera.h>
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void draw_vertices(std::vector<glm::vec3> triangle);
float rand_float(float a, float b);
void draw_cube(float width, float height, float depth, glm::vec3 bottom_right);
std::vector<glm::vec3> form_square(float width, float height, glm::vec3 bottom_left, glm::vec3 angles);
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
std::vector<std::vector<glm::vec3>> drawn_vertices;

Camera globalCamera;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    Shader ourShader("shaders/shader.vs", "shaders/shader.fs"); // you can name your shader files however you like

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    
    std::vector<glm::vec3> vertices = {
        glm::vec3(0.0f, 0.0f, 0.0f),
    };

    drawn_vertices = {
        vertices
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_DYNAMIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
    
    glfwSetKeyCallback(window, key_callback);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float const vecSize = sizeof(float) * 3;
    double lastInput = glfwGetTime();
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // change deltaTime
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        

        // render the triangle
        ourShader.use();
        
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //view = glm::translate(view, globalCamera.position);
        view = globalCamera.view;
        projection = glm::perspective(glm::radians(60.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        ourShader.setMat4("model", model);
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);

        
        for (const auto& verts : drawn_vertices) {

            glBufferData(GL_ARRAY_BUFFER, verts.size() * vecSize, &verts[0], GL_DYNAMIC_DRAW);
            glDrawArrays(GL_TRIANGLES, 0, verts.size());
        }

        glBindVertexArray(VAO);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}


float rand_float(float a, float b)
{
    return ((b - a) * ((float)rand() / RAND_MAX)) + a;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    float movementSpeed = 0.1f;

    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_L:
                draw_vertices({
                    glm::vec3(rand_float(-1, 1), rand_float(-1, 1), 0.0f),
                    glm::vec3(rand_float(-1, 1), rand_float(-1, 1), 0.0f),
                    glm::vec3(rand_float(-1, 1), rand_float(-1, 1), 0.0f),
                    });
                break;
            
            case GLFW_KEY_E:
                for (int i = 0; i <= 10; i++) {
                    draw_cube(0.5, 0.5, 0.5, glm::vec3(rand_float(-5, 5), rand_float(-5, 5), rand_float(-5, 5)));
                }
                break;

            case GLFW_KEY_W:
                globalCamera.move("forward");
                break;

            case GLFW_KEY_A:
                globalCamera.move("left");
                break;

            case GLFW_KEY_S:
                globalCamera.move("back");
                break;

            case GLFW_KEY_D:
                globalCamera.move("right");
                break;

            case GLFW_KEY_LEFT:
                globalCamera.changeDirection(glm::vec3(10.0f, 0.0f, 0.0f));
                break;

            case GLFW_KEY_RIGHT:
                globalCamera.changeDirection(glm::vec3(-10.0f, 0.0f, 0.0f));
                break;

            case GLFW_KEY_UP:
                globalCamera.changeDirection(glm::vec3(0.0f, 10.0f, 0.0f));
                break;

            case GLFW_KEY_DOWN:
                globalCamera.changeDirection(glm::vec3(0.0f, -10.0f, 0.0f));
                break;


        }
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void draw_cube(float width, float height, float depth, glm::vec3 bottom_left) {
    // Offset each face to the correct position
    std::vector<glm::vec3> face1 = form_square(width, height, bottom_left + glm::vec3(0, 0, depth), glm::vec3(0, 0, 0));        // Front face
    std::vector<glm::vec3> face2 = form_square(width, height, bottom_left + glm::vec3(width, 0, 0), glm::vec3(0, 180, 0));                               // Back face
    std::vector<glm::vec3> face3 = form_square(width, depth, bottom_left + glm::vec3(0, height, depth), glm::vec3(-90, 0, 0));      // Top face
    std::vector<glm::vec3> face4 = form_square(width, depth, bottom_left, glm::vec3(90, 0, 0));                                 // Bottom face
    std::vector<glm::vec3> face5 = form_square(depth, height, bottom_left + glm::vec3(width, 0, 0), glm::vec3(0, -90, 0));      // Right face
    std::vector<glm::vec3> face6 = form_square(depth, height, bottom_left + glm::vec3(0, 0, depth), glm::vec3(0, 90, 0));                                // Left face

    std::vector<std::vector<glm::vec3>> faces = {
        face1, face2, face3, face4, face5, face6
    };

    // Draw each face
    for (const auto& face : faces) {
        draw_vertices(face);
    }
}

// Returns a vector of vertices that form a square with given width, height, and bottom_right coord
std::vector<glm::vec3> form_square(float width, float height, glm::vec3 bottom_left, glm::vec3 angles) {
    // Create initial vertices of the square relative to (0, 0, 0)
    std::vector<glm::vec3> verts = {
        glm::vec3(0, 0, 0),                   // bottom left
        glm::vec3(width, 0, 0),               // bottom right
        glm::vec3(0, height, 0),              // top left
        glm::vec3(width, height, 0)           // top right
    };

    // Create rotation matrices for each axis
    glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), glm::radians(angles.x), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), glm::radians(angles.y), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), glm::radians(angles.z), glm::vec3(0.0f, 0.0f, 1.0f));

    // Combine rotations
    glm::mat4 rotationMatrix = rotationZ * rotationY * rotationX;

    // Translate each vertex, rotate, and then move to bottom_left position
    for (auto& vert : verts) {
        glm::vec4 rotatedVert = rotationMatrix * glm::vec4(vert, 1.0f); // Rotate
        vert = glm::vec3(rotatedVert) + bottom_left;                    // Apply translation to bottom_left
    }

    // Return vertices as triangles to form the square
    return {
        verts[0], verts[1], verts[2],  // First triangle
        verts[2], verts[1], verts[3]   // Second triangle
    };
}

// Adds a vector of vertices to the vertices list to be drawn next frame...
void draw_vertices(std::vector<glm::vec3> verts) {
    drawn_vertices.push_back(verts);
}