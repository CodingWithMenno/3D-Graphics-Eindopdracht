#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <ostream>

#include "stb_image.h"
#include "entities/Player.h"

#include "models/Model.h"
#include "renderEngine/Loader.h"
#include "renderEngine/ObjLoader.h"
#include "renderEngine/Renderer.h"
#include "shaders/StaticShader.h"
#include "toolbox/Toolbox.h"

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

static double updateDelta();

static GLFWwindow* window;


int main(void)
{
	#pragma region OPENGL_SETTINGS
    if (!glfwInit())
        throw "Could not inditialize glwf";
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGT, "Eindopdracht - Menno Bil", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        throw "Could not initialize glwf";
    }
    glfwMakeContextCurrent(window);
    glewInit();
    glGetError();
	#pragma endregion

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
	    if (key == GLFW_KEY_ESCAPE)
	        glfwSetWindowShouldClose(window, true);
    });

    std::cout << "Loading..." << std::endl;
	
    models::RawModel groundRawModel = renderEngine::LoadObjModel("res/Ground.obj");
    models::ModelTexture groundTexture = { renderEngine::loader::LoadTexture("res/Texture.png") };
    models::TexturedModel groundModel = { groundRawModel, groundTexture };
    entities::Entity ground(groundModel, glm::vec3(0, -35, 0), glm::vec3(0, 0, 0), 50);
    entities::Light sun(glm::vec3(0, 1000, -7000), glm::vec3(5, 5, 5));

    models::RawModel playerRawModel = renderEngine::LoadObjModel("res/Bee.obj");
    models::ModelTexture playerTexture = { renderEngine::loader::LoadTexture("res/Texture.png") };
    models::TexturedModel playerModel = { playerRawModel, playerTexture };
    entities::Player player(playerModel, glm::vec3(0, 5, -20), 1);
	
    shaders::StaticShader shader;
    shader.init();
    renderEngine::renderer::Init(shader);

    entities::Camera camera(glm::vec3(0, 20, 0), glm::vec3(0, 0, 0));

    std::cout << "Ready" << std::endl;
	
	// Main game loop
	while (!glfwWindowShouldClose(window))
	{
        // Update
        const double delta = updateDelta();

        camera.move(window, delta);
        player.move(window, delta);

		// Render
        renderEngine::renderer::Prepare();
        shader.start();
        shader.loadSkyColor(renderEngine::renderer::SKY_COLOR);
        shader.loadLight(sun);
        shader.loadViewMatrix(camera);
		
        renderEngine::renderer::Render(ground, shader);
        renderEngine::renderer::Render(player, shader);

		// Finish up
        shader.stop();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Clean up
    shader.cleanUp();
    renderEngine::loader::CleanUp();
	glfwTerminate();
    return 0;
}

static double updateDelta()
{
    double currentTime = glfwGetTime();
    static double lastFrameTime = currentTime;
    double deltaTime = (currentTime - lastFrameTime);
    lastFrameTime = currentTime;
    return deltaTime;
}