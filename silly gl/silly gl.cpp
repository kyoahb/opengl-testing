#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <Camera.h>
#include <Input.h>
#include <Renderer.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// settings
Camera globalCamera;
InputManager inputManager(&globalCamera);

std::vector<GLint> firsts;
std::vector<GLsizei> counts;
bool verticesUpdated = true;
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
std::vector<std::vector<glm::vec3>> drawn_vertices = { std::vector<glm::vec3>{glm::vec3(0.0f, 0.0f, 0.0f)} };
float const vecSize = sizeof(float) * 3;
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

    // tell GLFW to capture our mouse
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // build and compile our shader program
    // ------------------------------------
    Renderer renderer(&globalCamera);
	inputManager.setRenderer(&renderer);
    
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

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
    double lastInput = glfwGetTime();
    int frames = 0;
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // change deltaTime
        float currentFrame = glfwGetTime();
        frames++;
		if (currentFrame - lastInput > 1) {
            lastInput = currentFrame;
            std::cout << "FPS: " << frames << "\n";
            frames = 0;
		}
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        inputManager.update(deltaTime);

        // render
        // ------
        renderer.render();
        

        glBindVertexArray(VAO);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
        glFlush();
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

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	inputManager.key_callback(window, key, scancode, action, mods);
    /*
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
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
            globalCamera.changeDirection(glm::vec3(0.0f, -10.0f, 0.0f)); // Yaw left
            break;

        case GLFW_KEY_RIGHT:
            globalCamera.changeDirection(glm::vec3(0.0f, 10.0f, 0.0f)); // Yaw right
            break;

        case GLFW_KEY_UP:
            globalCamera.changeDirection(glm::vec3(-10.0f, 0.0f, 0.0f)); // Pitch up
            break;

        case GLFW_KEY_DOWN:
            globalCamera.changeDirection(glm::vec3(10.0f, 0.0f, 0.0f)); // Pitch down
            break;
        }
    }
    */
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	inputManager.mouse_callback(window, xpos, ypos);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}