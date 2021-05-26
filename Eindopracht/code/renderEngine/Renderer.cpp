#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "../models/Model.h"
#include "Renderer.h"
#include "../toolbox/Toolbox.h"

namespace renderEngine
{
	namespace renderer
	{
		static const float FOV = 70.0f;
		static const float NEAR_PLANE = 0.01f;
		static const float FAR_PLANE = 1000.0f;

		/*
			Renders all the entities
		 */
		void RenderEntities(std::vector<entities::Entity*>& entities, entities::Light& sun, entities::Camera& camera,
			shaders::StaticShader& shader)
		{
			shader.start();
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
			This function will load the projectionMatrix into the shader
		 */
		void Init(shaders::StaticShader& shader)
		{
			// Faces which are not facing the camera are not rendered
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			
			const glm::mat4 projectionMatrix = 
				glm::perspective(glm::radians(FOV), (WINDOW_WIDTH / WINDOW_HEIGT), NEAR_PLANE, FAR_PLANE);

			// Load the projectionmatrix into the shader
			shader.start();
			shader.loadProjectionMatrix(projectionMatrix);
			shader.stop();
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
			This function will Render a Model on the screen.
		*/
		void Render(entities::Entity& entity, shaders::StaticShader& shader)
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
	}
}