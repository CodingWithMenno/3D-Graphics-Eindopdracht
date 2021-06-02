#pragma once

#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include "Entity.h"

namespace entities
{
	class CloudGroup
	{
	private:
		const float movementSpeed = 3;
		const float modelSize = 0.05f;
		const float totalClouds;
		
		models::TexturedModel cloudModel;
		std::vector<std::shared_ptr<Entity>> clouds;

		glm::vec3 center;
		glm::vec2 size;
		glm::vec2 maxCoords;

	public:
		CloudGroup(glm::vec3 spawnPlace, glm::vec2 size, float totalClouds);
		~CloudGroup();
		
		void update(const double delta);

		std::vector<std::shared_ptr<Entity>> GetEntities() { return clouds; }
	};
}
