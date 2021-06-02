#pragma once

#include "../entities/Camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace toolbox
{
	#define WINDOW_WIDTH 1920.0f
	#define WINDOW_HEIGT 1080.0f

	/*
	 * This function creates a model matrix for an entity
	 */
	glm::mat4 CreateModelMatrix(glm::vec3 translation, glm::vec3 rotation, float scale);

	/*
	 * This function creates a view matrix of the camera
	 */
	glm::mat4 CreateViewMatrix(entities::Camera& camera);

	/*
	 * This function will clamp the value between min and max
	 */
	float Clamp(float value, float min, float max);

	/*
	 * This function will return a value between min and max
	 */
	int Random(const int min, const int max);
}
