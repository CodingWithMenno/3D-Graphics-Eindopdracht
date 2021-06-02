#pragma once

#include "../entities/Camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace toolbox
{
	#define WINDOW_WIDTH 1920.0f
	#define WINDOW_HEIGT 1080.0f
	
	glm::mat4 CreateModelMatrix(glm::vec3 translation, glm::vec3 rotation, float scale);

	glm::mat4 CreateViewMatrix(entities::Camera& camera);

	float Clamp(float value, float min, float max);

	int Random(const int min, const int max);
}
