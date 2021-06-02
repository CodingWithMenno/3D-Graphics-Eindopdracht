#pragma once

#include <vector>
#include "../entities/Entity.h"
#include "../water/WaterTile.h"
#include "../shaders/EntityShader.h"
#include "../shaders/WaterShader.h"

namespace renderEngine
{
	namespace renderer
	{
		static const glm::vec3 SKY_COLOR = { 0.9, 0.8, 0.2 };

		/*
			Call this function when starting the program
		 */
		void Init(shaders::EntityShader& entityShader, shaders::WaterShader& waterShader);
		
		/*
			Call this function before rendering. 
		*/
		void Prepare();

		/*
			Generic function to render all the entities (makes the code cleaner and easier to use).
		 */
		void RenderEntities(std::vector<entities::Entity*>& entities, entities::Light& sun, entities::Camera& camera, glm::vec4 clippingPlane, shaders::EntityShader& shader);

		
		/*
			Call this function when wanting to render an entity to the screen.
		*/
		void Render(entities::Entity& entity, shaders::EntityShader& shader);

		/*
			Call this function to render a water tile on the screen
		 */
		void Render(water::WaterTile& waterTile, entities::Camera& camera, entities::Light& sun, shaders::WaterShader& shader);
	}
}