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
#include "shaders/EntityShader.h"
#include "toolbox/Toolbox.h"
#include "water/WaterFrameBuffer.h"

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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
	    if (key == GLFW_KEY_ESCAPE)
	        glfwSetWindowShouldClose(window, true);
    });

    std::cout << "Loading..." << std::endl;

	// Entities
    std::vector<entities::Entity*> entities;
	
    models::RawModel groundRawModel = renderEngine::LoadObjModel("res/Ground.obj");
    models::ModelTexture groundTexture = { renderEngine::loader::LoadTexture("res/Texture.png") };
    models::TexturedModel groundModel = { groundRawModel, groundTexture };
    entities::Entity ground(groundModel, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 30);
    entities.push_back(&ground);
	
    models::RawModel playerRawModel = renderEngine::LoadObjModel("res/Bee.obj");
    models::ModelTexture playerTexture = { renderEngine::loader::LoadTexture("res/Texture.png") };
    models::TexturedModel playerModel = { playerRawModel, playerTexture };
    entities::Player player(playerModel, glm::vec3(0, 35, 0), 1);
    entities.push_back(&player);

    entities::Light sun(glm::vec3(500, 1000, -7000), glm::vec3(2.2, 2.2, 1));
	
    entities::Camera camera(player);

	
    // Water
    water::WaterTile waterTile = { glm::vec3(0, 23, 0), 80 };
    water::frameBuffer::Init();
	
	
    shaders::WaterShader waterShader;
    waterShader.init();

    shaders::EntityShader entityShader;
    entityShader.init();
	
    renderEngine::renderer::Init(entityShader, waterShader);

    std::cout << "Ready" << std::endl;
	
	// Main game loop
	while (!glfwWindowShouldClose(window))
	{
        // Update
        const double delta = updateDelta();

        camera.move(window, delta);
        player.move(window, delta);

		// Render
        water::frameBuffer::BindReflectionFBO();
        renderEngine::renderer::Prepare();
        renderEngine::renderer::RenderEntities(entities, sun, camera, entityShader);
        water::frameBuffer::UnbindCurrentFBO();
		
        renderEngine::renderer::Prepare();
        renderEngine::renderer::RenderEntities(entities, sun, camera, entityShader);
        renderEngine::renderer::Render(waterTile, camera, waterShader);
		
		// Finish up
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

    std::cout << "Exiting..." << std::endl;
	
	// Clean up
    entityShader.cleanUp();
    waterShader.cleanUp();
    water::frameBuffer::CleanUp();
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