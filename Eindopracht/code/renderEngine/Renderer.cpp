#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "../models/Model.h"
#include "../toolbox/Toolbox.h"
#include "Loader.h"
#include "Renderer.h"

namespace renderEngine
{
	namespace renderer
	{
		static const float FOV = 70.0f;
		static const float NEAR_PLANE = 0.01f;
		static const float FAR_PLANE = 1000.0f;

		static models::RawModel waterQuad;

		/*
			This function will load the projectionMatrix into the shaders
		 */
		void Init(shaders::EntityShader& entityShader, shaders::WaterShader& waterShader)
		{
			// Faces which are not facing the camera are not rendered
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			
			const glm::mat4 projectionMatrix = 
				glm::perspective(glm::radians(FOV), (WINDOW_WIDTH / WINDOW_HEIGT), NEAR_PLANE, FAR_PLANE);

			// Load the projectionmatrix into the shaders
			entityShader.start();
			entityShader.loadProjectionMatrix(projectionMatrix);
			entityShader.stop();

			waterShader.start();
			waterShader.loadProjectionMatrix(projectionMatrix);
			waterShader.stop();

			// Only x and z positions, the y is set to 0 in the water vertex shader
			std::vector<float> waterQuadVertices = { -1, -1, -1, 1, 1, -1, 1, -1, -1, 1, 1, 1 };
			waterQuad = loader::LoadToVAO(waterQuadVertices, 2);
		}

		/*
		 	This function will clear the screen.
		*/
		void Prepare()
		{
			glEnable(GL_DEPTH_TEST);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(SKY_COLOR.r, SKY_COLOR.g, SKY_COLOR.b, 1.0f);
		}

		/*
			Renders all the entities
		 */
		void RenderEntities(std::vector<entities::Entity*>& entities, entities::Light& sun, entities::Camera& camera,
			glm::vec4 clippingPlane, shaders::EntityShader& shader)
		{
			shader.start();
			shader.loadClippingPlane(clippingPlane);
			shader.loadSkyColor(SKY_COLOR);
			shader.loadLight(sun);
			shader.loadViewMatrix(camera);

			for (entities::Entity* entity : entities)
			{
				Render(*entity, shader);
			}

			shader.stop();
		}

		/*
			This function will Render a Model on the screen.
		*/
		void Render(entities::Entity& entity, shaders::EntityShader& shader)
		{
			const models::TexturedModel model = entity.getModel();
			const models::RawModel rawModel = model.rawModel;
			const models::ModelTexture texture = model.texture;

			// Enable the model
			glBindVertexArray(rawModel.vaoID);

			// Enable the VBO's from the model (VAO)
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);

			// Load the transformation of the model into the shader
			const glm::mat4 modelMatrix = toolbox::CreateModelMatrix(entity.getPosition(), entity.getRotation(), entity.getScale());
			shader.loadModelMatrix(modelMatrix);
			shader.loadShineVariables(texture.shineDamper, texture.reflectivity);
			
			// Draw the model
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, model.texture.textureID);
			glDrawElements(GL_TRIANGLES, rawModel.vertexCount, GL_UNSIGNED_INT, 0);

			// Disable the VBO's and model
			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);
			glBindVertexArray(0);
		}

		/*
			This function will Render a water tile on the screen.
		*/
		void Render(water::WaterTile& waterTile, entities::Camera& camera, shaders::WaterShader& shader)
		{
			// Start the render and load the camera's perspective into the shader
			shader.start();
			shader.loadViewMatrix(camera);

			// Load up the model and positions VBO
			glBindVertexArray(waterQuad.vaoID);
			glEnableVertexAttribArray(0);

			// Load the model matrix into the shader
			const glm::mat4 modelMatrix = toolbox::CreateModelMatrix(waterTile.position, glm::vec3(0, 0, 0), waterTile.size);
			shader.loadModelMatrix(modelMatrix);

			// Render
			glDrawArrays(GL_TRIANGLES, 0, waterQuad.vertexCount);

			// Finish up the render
			glDisableVertexAttribArray(0);
			glBindVertexArray(0);
			shader.stop();
		}
	}
}