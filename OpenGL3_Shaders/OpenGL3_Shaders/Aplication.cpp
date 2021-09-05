#include "Aplication.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Aplication::Run()
{
    GLFWwindow* window;

    /* Initialize the glfw */
    if (!glfwInit())
        return;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    glViewport(0, 0, 640, 480);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Init();

    while (!glfwWindowShouldClose(window))
    {
        Update();
        Render();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
}

void Aplication::Init()
{
    float vertices[] =
    {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
    };

    unsigned int indices[] =
    {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    std::unique_ptr<VertexBuffer> vbo(new VertexBuffer(vertices, sizeof(vertices)));

    vbo->Bind();

    vbo->SetLayout(
        {
            {"aPos", Float3},
            {"aColor", Float3}
        }
    );

    auto ibo = std::make_unique<IndexBuffer>(indices, sizeof(indices));

    _va = std::make_unique<VertexArray>();

    _va->SetVertexBuffer(std::move(vbo));

    _va->SetIndexBuffer(std::move(ibo));

    _va->Bind();

    _shader = std::make_unique<Shader>("res\\vertex.glsl", "res\\fragment.glsl");
}

void Aplication::Update()
{
}

void Aplication::Render()
{
    glClearColor(0.1f, 0.2f, 0.3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    _shader->use();
    _va->Bind();

    glDrawArrays(GL_TRIANGLES, 0, 3);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
