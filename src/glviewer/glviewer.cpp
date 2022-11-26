#include "glviewer.hpp"
#include <cstdio>

#include <GLFW/glfw3.h>

namespace renderme::glviewer
{

    void RenderScenceCB()
    {

    }

    auto GL_Viewer::main_loop()->void
    {
        GLFWwindow* window;

        /* Initialize the library */
        if (!glfwInit())
            return;

        /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
        if (!window) {
            glfwTerminate();
            return;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(window);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)) {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        glfwTerminate();
        return;
    }


}