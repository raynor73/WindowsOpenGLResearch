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
#include <platform_dependent/windows/windows_read_only_fs_abstraction.h>
#include <platform_dependent/windows/windows_mesh_loader.h>
#include <platform_dependent/windows/windows_fs_abstraction.h>
#include <platform_dependent/windows/windows_logger.h>
#include <platform_dependent/windows/windows_bitmap_loader.h>
#include <platform_dependent/windows/windows_opengl_shader_source_loader.h>
#include <rendering_engine/opengl_mesh_renderer_factory.h>
#include <rendering_engine/rendering_engine.h>
#include <platform_dependent/windows/windows_keyboard_input.h>
#include <platform_dependent/windows/windows_app.h>
#include <platform_dependent/windows/windows_mouse_input.h>
#include <locale>

#define CONSOLE_BUFFER_SIZE 1024

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define WINDOW_DENSITY_FACTOR 1

using namespace GameEngine;
using namespace GameEngine::RenderingEngine;
using namespace Windows::Utils;
using namespace std;

static shared_ptr<DevSceneManager> g_sceneManager;
static shared_ptr<GameEngine::RenderingEngine::RenderingEngine> g_renderingEngine;
static shared_ptr<WindowsKeyboardInput> g_keyboardInput;
static shared_ptr<WindowsMouseInput> g_mouseInput;
static shared_ptr<WindowsApp> g_app;

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
        g_mouseInput->update();
        auto activeScene = g_sceneManager->activeScene();
        if (activeScene != nullptr) {
            activeScene->update();
            g_renderingEngine->render(*activeScene);
        }

        /* Render here */
        /*glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.0f, 0.5f);
        glVertex2f(0.5f, -0.5f);
        glEnd();*/

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        if (g_app->isExitRequested()) {
            glfwDestroyWindow(window);
        }
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
    L::setLogger(make_shared<WindowsLogger>());

    auto serviceLocator = make_shared<ServiceLocator>();

    g_app = make_shared<WindowsApp>();
    serviceLocator->provide(g_app);
    serviceLocator->provide(make_shared<TimeProvider>());
    serviceLocator->provide(make_shared<TimeManager>(serviceLocator));
    serviceLocator->provide(make_shared<WindowsRenderingWindowInfoProvider>(float(WINDOW_WIDTH), float(WINDOW_HEIGHT), float(WINDOW_DENSITY_FACTOR)));
    serviceLocator->provide(make_shared<WindowsReadOnlyFsAbstraction>());
    serviceLocator->provide(make_shared<WindowsFsAbstraction>(serviceLocator));
    serviceLocator->provide(make_shared<WindowsMeshLoader>(serviceLocator));
    serviceLocator->provide(make_shared<UnitsConverter>(serviceLocator));
    serviceLocator->provide(make_shared<SceneHierarchyLoader>(serviceLocator));
    serviceLocator->provide(make_shared<WindowsBitmapLoader>(serviceLocator));

    auto openGLErrorDetector = make_shared<OpenGLErrorDetector>();
    serviceLocator->provide(make_shared<OpenGLTexturesRepository>(
        serviceLocator->renderingWindowInfoProvider(),
        serviceLocator->bitmapLoader(),
        openGLErrorDetector
    ));

    auto openGLGeometryBuffersStorage = make_shared<OpenGLGeometryBuffersStorage>(openGLErrorDetector);
    serviceLocator->provide(make_shared<OpenGLMeshRendererFactory>(
        openGLGeometryBuffersStorage,
        reinterpret_cast<OpenGLTexturesRepository*>(serviceLocator->texturesRepository()),
        openGLErrorDetector
    ));

    g_renderingEngine = make_shared<GameEngine::RenderingEngine::RenderingEngine>(
        openGLErrorDetector,
        serviceLocator->unitsConverter(),
        make_shared<OpenGLShadersRepository>(openGLErrorDetector),
        make_shared<OpenGLShaderSourcePreprocessor>(make_shared<WindowsOpenGLShaderSourceLoader>(serviceLocator)),
        openGLGeometryBuffersStorage,
        reinterpret_cast<OpenGLTexturesRepository*>(serviceLocator->texturesRepository())
    );

    g_sceneManager = make_shared<DevSceneManager>(serviceLocator);
    serviceLocator->provide(g_sceneManager);

    serviceLocator->provide(g_keyboardInput);
    serviceLocator->provide(g_mouseInput);

    g_sceneManager->requestHelloWorldSceneStart();
}

static void characterCallback(GLFWwindow* window, unsigned int codepoint)
{
    // not used yet
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch (key) {
    case GLFW_KEY_ESCAPE:
        if (action == GLFW_PRESS) {
            g_keyboardInput->setKeyPressed(KeyboardInput::KEY_ESC, true);
        }
        else if (action == GLFW_RELEASE) {
            g_keyboardInput->setKeyPressed(KeyboardInput::KEY_ESC, false);
        }
        break;

    case GLFW_KEY_W:
        if (action == GLFW_PRESS) {
            g_keyboardInput->setKeyPressed(KeyboardInput::KEY_W, true);
        } else if (action == GLFW_RELEASE) {
            g_keyboardInput->setKeyPressed(KeyboardInput::KEY_W, false);
        }
        break;

    case GLFW_KEY_S:
        if (action == GLFW_PRESS) {
            g_keyboardInput->setKeyPressed(KeyboardInput::KEY_S, true);
        } else if (action == GLFW_RELEASE) {
            g_keyboardInput->setKeyPressed(KeyboardInput::KEY_S, false);
        }
        break;

    case GLFW_KEY_A:
        if (action == GLFW_PRESS) {
            g_keyboardInput->setKeyPressed(KeyboardInput::KEY_A, true);
        } else if (action == GLFW_RELEASE) {
            g_keyboardInput->setKeyPressed(KeyboardInput::KEY_A, false);
        }
        break;

    case GLFW_KEY_D:
        if (action == GLFW_PRESS) {
            g_keyboardInput->setKeyPressed(KeyboardInput::KEY_D, true);
        } else if (action == GLFW_RELEASE) {
            g_keyboardInput->setKeyPressed(KeyboardInput::KEY_D, false);
        }
        break;
    }
}

static void initInput(GLFWwindow* window) {
    g_keyboardInput = make_shared<WindowsKeyboardInput>();
    g_mouseInput = make_shared<WindowsMouseInput>(window);

    glfwSetCharCallback(window, characterCallback);
    glfwSetKeyCallback(window, keyCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
    
    initInput(window);

    initGame();

    mainLoop(window);

    glfwTerminate();

    return EXIT_SUCCESS;
}
