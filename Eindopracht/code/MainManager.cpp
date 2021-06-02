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

    models::RawModel sunRawModel = renderEngine::LoadObjModel("res/Sun.obj");
    models::ModelTexture sunTexture = { renderEngine::loader::LoadTexture("res/Texture.png") };
    models::TexturedModel sunModel = { sunRawModel, sunTexture };
    entities::Entity sunEntity(sunModel, glm::vec3(110, 150, -200), glm::vec3(25, -23, 0), 10);
    entities.push_back(&sunEntity);

    entities::Light sun(glm::vec3(100, 140, -170), glm::vec3(1.7, 1.2, 0.4));
	
    entities::Camera camera(player);

	
    // Water
    water::WaterTile waterTile = { glm::vec3(0, 19, 0), 80 };
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
		
		// Enable clipping plane 0
		// (so only everything above the water will get reflect and only everything under the water will get refracted)
        glEnable(GL_CLIP_DISTANCE0);

		// Render reflection of the water
        water::frameBuffer::BindReflectionFBO();
        float distance = 2 * (camera.getPosition().y - waterTile.position.y);
        camera.getPositionRef().y -= distance;
        camera.invertPitch();
        renderEngine::renderer::Prepare();
        renderEngine::renderer::RenderEntities(entities, sun, camera, glm::vec4(0, 1, 0, -waterTile.position.y + 0.2f), entityShader);
        camera.getPositionRef().y += distance;
        camera.invertPitch();
		
		// Render refraction of the water
        water::frameBuffer::BindRefractionFBO();
        renderEngine::renderer::Prepare();
        renderEngine::renderer::RenderEntities(entities, sun, camera, glm::vec4(0, -1, 0, waterTile.position.y), entityShader);

		// Render the normal scene
        glDisable(GL_CLIP_DISTANCE0);
		water::frameBuffer::UnbindCurrentFBO();
        renderEngine::renderer::Prepare();
        renderEngine::renderer::RenderEntities(entities, sun, camera, glm::vec4(0, 0, 0, 0), entityShader);
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