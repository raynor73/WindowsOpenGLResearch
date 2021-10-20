// GameEngine.cpp : Defines the entry point for the application.
//

#include <GL/glew.h>
#include "framework.h"
#include "GameEngine.h"
#include <platform_dependent/windows/utils.h>
#include <game/dev_scene_manager.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <game_engine/service_locator.h>
#include <platform_dependent/windows/windows_rendering_window_info_provider.h>

#define CONSOLE_BUFFER_SIZE 1024

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define WINDOW_DENSITY_FACTOR 1

using namespace GameEngine;
using namespace Windows::Utils;
using namespace std;

static shared_ptr<DevSceneManager> g_sceneManager;

static bool setupConsolse(HINSTANCE hInstance) {
    if (!createNewConsole(CONSOLE_BUFFER_SIZE)) {
        showDialog(
            getString(hInstance, IDS_GENERIC_ERROR_TITLE).get(),
            getString(hInstance, IDS_ERROR_OPENING_CONSOLE).get()
        );
        return false;
    }

    return true;
}

static void mainLoop(GLFWwindow* window) {
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Game logic */
        auto activeScene = g_sceneManager->activeScene();
        if (activeScene != nullptr) {
            activeScene->update();
        }

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.0f, 0.5f);
        glVertex2f(0.5f, -0.5f);
        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
}

static GLFWwindow* initOpenGL(HINSTANCE hInstance) {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) {
        showDialog(
            getString(hInstance, IDS_GENERIC_ERROR_TITLE).get(),
            getString(hInstance, IDS_ERROR_INITIALIZING_GLFW).get()
        );
        return nullptr;
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello World", NULL, NULL);
    if (!window) {
        showDialog(
            getString(hInstance, IDS_GENERIC_ERROR_TITLE).get(),
            getString(hInstance, IDS_ERROR_CREATING_MAIN_WINDOW).get()
        );

        glfwTerminate();
        return nullptr;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        showDialog(
            getString(hInstance, IDS_GENERIC_ERROR_TITLE).get(),
            getString(hInstance, IDS_ERROR_INITIALIZING_GLEW).get()
        );
        return nullptr;
    }

    return window;
}

static void initGame() {
    auto serviceLocator = make_shared<ServiceLocator>();

    serviceLocator->provide(make_shared<TimeManager>(make_shared<TimeProvider>()));
    serviceLocator->provide(make_shared<WindowsRenderingWindowInfoProvider>(float(WINDOW_WIDTH), float(WINDOW_HEIGHT), float(WINDOW_DENSITY_FACTOR)));

    g_sceneManager = make_shared<DevSceneManager>(serviceLocator);
    serviceLocator->provide(g_sceneManager);

    g_sceneManager->requestHelloWorldSceneStart();
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    if (!setupConsolse(hInstance)) {
        return EXIT_FAILURE;
    }

    GLFWwindow* window = initOpenGL(hInstance);
    if (window == NULL) {
        return EXIT_FAILURE;
    }

    initGame();

    mainLoop(window);

    glfwTerminate();

    return EXIT_SUCCESS;
}
