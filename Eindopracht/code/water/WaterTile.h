#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace water
{
	/*
	 * This struct represents a water tile
	 */
	struct WaterTile
	{
		glm::vec3 position;
		float size;
	};
}
