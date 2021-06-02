#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Entity.h"

namespace entities
{
	class Player : public Entity
	{
	private:
		static const int RUN_SPEED = 20;
		static const int TURN_SPEED = 160;

		float currentSpeed = 0;
		float currentTurnSpeed = 0;
	
	public:
		Player(const models::TexturedModel& model, const glm::vec3& position, float scale);

		void update(GLFWwindow* window, const double delta);

	private:
		void checkInputs(GLFWwindow* window);
	};
}
