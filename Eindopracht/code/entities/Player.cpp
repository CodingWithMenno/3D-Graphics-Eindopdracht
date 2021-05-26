#include "Player.h"

namespace entities
{
	Player::Player(const models::TexturedModel& model, const glm::vec3& position, float scale)
		: Entity(model, position, glm::vec3(0, 0, 0), scale)
	{ }

	void Player::move(GLFWwindow* window, const double delta)
	{
		checkInputs(window);
		increaseRotation(glm::vec3(0, currentTurnSpeed * delta, 0));

		float distance = currentSpeed * delta;
		float dz = distance * glm::cos(glm::radians(rotation.y));
		float dx = distance * glm::sin(glm::radians(rotation.y));
		increasePosition(glm::vec3(dx, 0, dz));
	}

	void Player::checkInputs(GLFWwindow* window)
	{
		currentSpeed = 0;
		currentTurnSpeed = 0;
		
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			currentSpeed += RUN_SPEED;
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			currentSpeed -= RUN_SPEED;
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			currentTurnSpeed -= TURN_SPEED;
		}

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			currentTurnSpeed += TURN_SPEED;
		}
	}
}
