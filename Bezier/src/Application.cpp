#include <iostream>
#include <thread>
#include <Server/Server.h>
#include <Server/Client.h>

#include "Scene/Scene.h"

#include "TextureInit/TextureInit.h"
#include "OpenGL/Resolution.h"
#include <Scene/NewMenu/NewMenu.h>

#include "ImGuiFunctionality/ImGuiFunctionality.h"

int main(void)
{
    double xpos, ypos;
    //1000,640
    //1280,960
    //1920,1080 - 90
    ::ShowWindow(::GetConsoleWindow(), SW_SHOW); 
    if (!glfwInit()) return -1;
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    Resolution::Init(mode->width, mode->height);
    //std::cout << Resolution::WIDTH << "/" << Resolution::HEIGHT << "\n";
    GLFWwindow* globalWindow;
    //glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    globalWindow = glfwCreateWindow(static_cast<int>(Resolution::WIDTH), static_cast<int>(Resolution::HEIGHT), "Normal program", NULL/*glfwGetPrimaryMonitor()*/, NULL);
    glfwSetInputMode(globalWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    if (!globalWindow)
    {
        glfwTerminate();
        return -1;
    }
    

    ButtonFunctionality::SetWindow(globalWindow);

    glfwMakeContextCurrent(globalWindow);

    glfwSwapInterval(1);

    glewInit();

    glfwMaximizeWindow(globalWindow);

    ImGuiFunctionality::Init();

    TextureInit::InitTextures();
    ButtonFunctionality::SetUpScenes();
    
    Mesh::SetupProj();
    
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    while (!glfwWindowShouldClose(globalWindow))
    {
        glfwPollEvents();

        glfwGetCursorPos(globalWindow, &xpos, &ypos);
        ypos = Resolution::HEIGHT - ypos;
        
        if (ButtonFunctionality::shouldPerformButtonCheck) {
            ButtonFunctionality::PerformButtonCheck();
        }
        glfwSetKeyCallback(globalWindow, ButtonFunctionality::KeyCallback);
        glfwSetScrollCallback(globalWindow, ButtonFunctionality::ScrollCallback);
        
        glfwSetMouseButtonCallback(globalWindow, ButtonFunctionality::MouseButtonCallback);

		if (ButtonFunctionality::shouldSetup) {
			ButtonFunctionality::getCurrentSceneObject()->SetUp();
			ButtonFunctionality::shouldSetup = false;
		}

        ButtonFunctionality::getCurrentSceneObject()->Update(static_cast<float>(xpos), static_cast<float>(ypos));
        ImGuiFunctionality::Update(static_cast<float>(xpos), static_cast<float>(ypos));
        ButtonFunctionality::getCurrentSceneObject()->Draw();
        ImGuiFunctionality::Draw();

        glfwSwapBuffers(globalWindow);
    }
    return 0;
    ImGuiFunctionality::ShutDown();
    glfwTerminate();
}
