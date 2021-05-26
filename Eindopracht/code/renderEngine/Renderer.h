#pragma once

#include <vector>
#include "../entities/Entity.h"
#include "../shaders/StaticShader.h"

namespace renderEngine
{
	namespace renderer
	{
		static const glm::vec3 SKY_COLOR = { 0.5f, 0.8f, 0.9f };

		/*
			Generic function to render all the entities (makes the code cleaner and easier to use).
		 */
		void RenderEntities(std::vector<entities::Entity*>& entities, entities::Light& sun, entities::Camera& camera, shaders::StaticShader& shader);

		/*
			Call this function when starting the program
		 */
		void Init(shaders::StaticShader& shader);
		
		/*
			Call this function before rendering. 
		*/
		void Prepare();

		/*
			Call this function when wanting to render an entity to the screen.
		*/
		void Render(entities::Entity& entity, shaders::StaticShader& shader);
	}
}