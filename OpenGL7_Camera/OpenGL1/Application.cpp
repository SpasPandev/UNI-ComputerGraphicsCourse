#include "Application.h"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include <stb_image/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    static bool firstMouse = true;
    static double lastX;
    static double lastY;
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    auto* app = (Application*)glfwGetWindowUserPointer(window);

    app-> _yaw += xoffset;
    app->_pitch += yoffset;

    if (app->_pitch > 89.0f)
        app->_pitch = 89.0f;
    if (app->_pitch < -89.0f)
        app->_pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(app->_yaw)) * cos(glm::radians(app->_pitch));
    direction.y = sin(glm::radians(app->_pitch));
    direction.z = sin(glm::radians(app->_yaw)) * cos(glm::radians(app->_pitch));
    app-> _cameraFront = glm::normalize(direction);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    auto* app = (Application*)glfwGetWindowUserPointer(window);

    app->_fov -= (float)yoffset;

    if (app->_fov < 1.0f)
        app->_fov = 1.0f;

    if (app->_fov > 45.0f)
        app->_fov = 45.0f;
}

void Application::ProcessInput(GLFWwindow* window, float dt)
{
    const float cameraSpeed = 2.5f * dt; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        _cameraPos += cameraSpeed * _cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        _cameraPos -= cameraSpeed * _cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        _cameraPos -= glm::normalize(glm::cross(_cameraFront, _cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        _cameraPos += glm::normalize(glm::cross(_cameraFront, _cameraUp)) * cameraSpeed;
}

void Application::Run()
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

    glfwSetWindowUserPointer(window, this);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetScrollCallback(window, scroll_callback);

    Init();

    float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame

	while (!glfwWindowShouldClose(window))
	{
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        ProcessInput(window, deltaTime);

        Update(deltaTime);
        Render();

        glfwSwapBuffers(window);
        glfwPollEvents();
	}

    glfwTerminate();
}

void Application::Init()
{
    //float vertices[] = {
    //    // positions          // colors           // texture coords
    //     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
    //     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    //    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    //    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    //};

    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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
            {"aTexCoords", Float2}
        }
    );

    auto ibo = std::make_unique<IndexBuffer>(indices, sizeof(indices));

    _va = std::make_unique<VertexArray>();

    _va->SetVertexBuffer(std::move(vbo));
    _va->SetIndexBuffer(std::move(ibo));
    _va->Bind();

    _shader = std::make_unique<Shader>("res\\vertex.glsl", "res\\fragment.glsl");

    _texture = std::make_unique<Texture>("res\\container.jpg");

    glEnable(GL_DEPTH_TEST);

}

void Application::Update(float dt)
{
    auto view = glm::lookAt(_cameraPos, _cameraPos + _cameraFront, _cameraUp);

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(_fov), 800.0f / 600.0f, 0.1f, 100.0f);

    _shader->setFloatMat4("view", view);
    _shader->setFloatMat4("projection", projection);

}

void Application::Render()
{
    glClearColor(0.1f, 0.2f, 0.3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
    _shader->use();
    _shader->setInt("ourTexture", 0);

    _va->Bind();

    _texture->Bind(0);

    glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    for (unsigned int i = 0; i < 10; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);

        model = glm::translate(model, cubePositions[i]);

        float angle = 20.0f * i + glfwGetTime() * 20.0f;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

        _shader->setFloatMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // glDrawArrays(GL_TRIANGLES, 0, 36);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
