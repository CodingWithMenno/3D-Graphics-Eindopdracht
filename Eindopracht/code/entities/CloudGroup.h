#pragma once

#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include "Entity.h"

namespace entities
{
	class Cloud : public Entity
	{
		
	};
	
	class CloudGroup
	{
	private:
		models::TexturedModel cloudModel;
		std::vector<Entity> clouds;

	public:
		CloudGroup(glm::vec3 spawnPlace, glm::vec2 size);

		void update(const double delta);
	};
}
