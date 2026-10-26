#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include "shader.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, float& move) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        move -= 0.001f;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        move += 0.001f;
    }
}

GLFWwindow* InitWindow(const std::string& name) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow* window = glfwCreateWindow(800, 600, name.c_str(), NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return NULL;
    }
    
    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return window;
}

int main()
{
    GLFWwindow* window = InitWindow("Cinema");
    if (window == NULL) {
        std::cerr << "Failed to initialize window.\n";
        return -1;
    }

    Shader ourShader("ourShader.vs", "ourShader.fs");
    
    float vertices[] = {
        // pos           // colour
        0.75f, 0.75f, 0.f,
        0.75f, -0.75f, 0.f,
        -0.75f, 0.75f, 0.f,
        -0.75f, -0.75f, 0.f,
    };

    // 12 triangles (2 per face) wound Counter-Clockwise decapitater (CCWD)
    unsigned int indices[] = {
        0, 1, 2,
        2, 1, 3
    };

    unsigned int EBO;
    glGenBuffers(1, &EBO);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attr
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    long long int frames = 0;
    auto start_time = glfwGetTime();
    float fps = 0;

    float move = 0.f;

    // while game active
    while(!glfwWindowShouldClose(window))
    {
        frames++;
        if (glfwGetTime() - start_time > 1)
        {
            fps = frames / (glfwGetTime() - start_time);

            std::cout << "fps = " << fps << "\n";
            start_time = glfwGetTime();
            frames = 0;
        }

        processInput(window, move);

        // Render out stuff.
        glClearColor(0.f, 0.5f, 0.5f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.use();
        ourShader.setFloat("move", move);
        ourShader.setFloat("u_time", glfwGetTime());

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    
    glfwTerminate();
    return 0;
}