#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_SSE42 // or GLM_FORCE_SSE42 if your processor supports it
#define GLM_FORCE_ALIGNED
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <Camera.h>
#include <Input.h>
#include <Renderer.h>
#include <Objects.h>
#include <ScriptManager.h>

#include <example.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// settings 
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// load globals
Camera globalCamera;
ObjectManager objectManager;
InputManager inputManager(&globalCamera, &objectManager);
ScriptManager scriptManager;
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

    // tell GLFW to capture our mouse and keyboard inputs
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // setup renderer and hook it into input manager (should be moved away to a unity scripting type system later, but for now this is ok)
    Renderer renderer(&objectManager, SCR_WIDTH, SCR_HEIGHT);
    renderer.setCamera(&globalCamera);

    scriptManager.registerScript(new ExampleScript());

    // Start scripts
    scriptManager.startScripts(&inputManager, &objectManager, &globalCamera, &renderer);

    double deltaTime = 0.0f;
    double lastFrame = 0.0f;
    double lastSecond = glfwGetTime();
    int frames = 0;
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // change deltaTime
        double currentFrame = glfwGetTime();
        frames++;
		if (currentFrame - lastSecond > 1) {
            // A full second has passed. Return all the frames that have passed between that time.
            lastSecond = currentFrame;
            std::cout << "FPS: " << frames << "\n";
            frames = 0;
		}
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

		// input
        inputManager.update(deltaTime);

        //update scripts
        scriptManager.updateScripts(deltaTime);

        // render
        // ------
        renderer.render();
        //glBindVertexArray(VAO);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
        glFlush();
    }
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// send key callback into inputManager
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	inputManager.key_callback(window, key, scancode, action, mods);
}

// send mouse callback into inputManager
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	inputManager.mouse_callback(window, xpos, ypos);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}