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
#include "entities/CloudGroup.h"

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
	// Turn on anti MSAA x4
    glfwWindowHint(GLFW_SAMPLES, 4);
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGT, "Eindopdracht - Menno Bil", NULL, NULL);
    glEnable(GL_MULTISAMPLE);
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

	// De scene opbouwen, ik weet het. Het ziet er niet uit
    std::vector<std::shared_ptr<entities::Entity>> entities;
	
    models::RawModel groundRawModel = renderEngine::LoadObjModel("res/Ground.obj");
    models::ModelTexture groundTexture = { renderEngine::loader::LoadTexture("res/Texture.png") };
    models::TexturedModel groundModel = { groundRawModel, groundTexture };
    std::shared_ptr<entities::Entity> ground(new entities::Entity(groundModel, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 30));
    entities.push_back(ground);

    models::RawModel treeRawModel = renderEngine::LoadObjModel("res/Tree.obj");
    models::ModelTexture treeTexture = { renderEngine::loader::LoadTexture("res/TreeTexture.png") };
    models::TexturedModel treeModel = { treeRawModel, treeTexture };
    entities.push_back(std::make_shared<entities::Entity>(treeModel, glm::vec3(50, 27, 50), glm::vec3(0, 0, 0), 1));
    entities.push_back(std::make_shared<entities::Entity>(treeModel, glm::vec3(-80, 27, 10), glm::vec3(0, 45, 0), 1));
    entities.push_back(std::make_shared<entities::Entity>(treeModel, glm::vec3(0, 27, 80), glm::vec3(0, 180, 0), 0.7));
    entities.push_back(std::make_shared<entities::Entity>(treeModel, glm::vec3(-10, 27, -80), glm::vec3(0, 180, 0), 1.2));
    entities.push_back(std::make_shared<entities::Entity>(treeModel, glm::vec3(55, 25, -10), glm::vec3(0, 20, 0), 0.5));
    entities.push_back(std::make_shared<entities::Entity>(treeModel, glm::vec3(-100, 27, -100), glm::vec3(0, -70, 0), 1));
    entities.push_back(std::make_shared<entities::Entity>(treeModel, glm::vec3(80, 30, -90), glm::vec3(0, -120, 0), 0.7));
	
    models::RawModel playerRawModel = renderEngine::LoadObjModel("res/Bee.obj");
    models::ModelTexture playerTexture = { renderEngine::loader::LoadTexture("res/Texture.png") };
    models::TexturedModel playerModel = { playerRawModel, playerTexture };
    std::shared_ptr <entities::Player> player(new entities::Player(playerModel, glm::vec3(0, 35, 0), 1));
    entities.push_back(player);

    entities::CloudGroup cloudGroup(glm::vec3(0, 100, 0), glm::vec2(230, 230), 7);
	
    models::RawModel sunRawModel = renderEngine::LoadObjModel("res/Sun.obj");
    models::ModelTexture sunTexture = { renderEngine::loader::LoadTexture("res/Texture.png") };
    sunTexture.emissionFactor = 0.1f;
    models::TexturedModel sunModel = { sunRawModel, sunTexture };
    std::shared_ptr <entities::Entity> sunEntity(new entities::Entity(sunModel, glm::vec3(110, 150, -200), glm::vec3(25, -23, 0), 10));
    entities.push_back(sunEntity);

    entities::Light sun(glm::vec3(100, 140, -170), glm::vec3(1.7, 1.2, 0.4));
	
    entities::Camera camera(*player);

	
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

        camera.update(window);
        player->update(window, delta);
        cloudGroup.update(delta);

		// Render

		// Get all the entities
        std::vector<std::shared_ptr<entities::Entity>> entitiesToRender;
        entitiesToRender.insert(entitiesToRender.begin(), entities.begin(), entities.end());
        std::vector<std::shared_ptr<entities::Entity>> clouds = cloudGroup.GetEntities();
		entitiesToRender.insert(entitiesToRender.begin(), clouds.begin(), clouds.end());
		
		// Enable clipping plane 0
		// (so only everything above the water will get reflect and only everything under the water will get refracted)
        glEnable(GL_CLIP_DISTANCE0);

		// Render reflection of the water
        water::frameBuffer::BindReflectionFBO();
        float distance = 2 * (camera.getPosition().y - waterTile.position.y);
        camera.getPositionRef().y -= distance;
        camera.invertPitch();
        renderEngine::renderer::Prepare();
        renderEngine::renderer::RenderEntities(entitiesToRender, sun, camera, glm::vec4(0, 1, 0, -waterTile.position.y + 0.2f), entityShader);
        camera.getPositionRef().y += distance;
        camera.invertPitch();
		
		// Render refraction of the water
        water::frameBuffer::BindRefractionFBO();
        renderEngine::renderer::Prepare();
        renderEngine::renderer::RenderEntities(entitiesToRender, sun, camera, glm::vec4(0, -1, 0, waterTile.position.y), entityShader);

		// Render the normal scene
        glDisable(GL_CLIP_DISTANCE0);
		water::frameBuffer::UnbindCurrentFBO();
        renderEngine::renderer::Prepare();
        renderEngine::renderer::RenderEntities(entitiesToRender, sun, camera, glm::vec4(0, 0, 0, 0), entityShader);
        renderEngine::renderer::Render(waterTile, camera, sun, waterShader);
		
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