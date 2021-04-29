#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "../tigl.h"

#include "models/RawModel.h"
#include "renderEngine/Loader.h"
#include "renderEngine/Renderer.h"

using tigl::Vertex;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

static GLFWwindow* window;

static void init();
static void update(double delta);
static double updateDelta();
static void draw();

int main(void)
{
    if (!glfwInit())
        throw "Could not initialize glwf";
    window = glfwCreateWindow(1400, 800, "Eindopdracht - Menno Bil", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        throw "Could not initialize glwf";
    }
    glfwMakeContextCurrent(window);
    glewInit();
    glGetError();

    tigl::init();

    init();

	while (!glfwWindowShouldClose(window))
	{
        renderEngine::renderer::prepare();
        double delta = updateDelta();

		update(delta);
		draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

    renderEngine::loader::cleanUp();
	glfwTerminate();
    return 0;
}

static double updateDelta()
{
    double currentTime = glfwGetTime();
    static double lastFrameTime = currentTime;
    double deltaTime = currentTime - lastFrameTime;
    lastFrameTime = currentTime;
    return deltaTime;
}

struct RawModel model = { 0, 0 };

static void init()
{
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, true);
    });

    std::vector<float> vertices = 
    {
      -0.5f, 0.5f, 0,
      -0.5f, -0.5f, 0,
      0.5f, -0.5f, 0,
      0.5f, 0.5f, 0
    };

    std::vector<int> indices =
    {
        0,1,3,
        3,1,2
    };

    model = renderEngine::loader::loadToVAO(vertices, indices);
}

static void update(double delta)
{
    
}

static void draw()
{
    renderEngine::renderer::render(model);
}