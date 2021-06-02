#include "../toolbox/Toolbox.h"
#include "Camera.h"

namespace entities
{
	Camera::Camera(Entity& entityToFollow) : entityToFollow(entityToFollow)
	{
		position = entityToFollow.getPosition();
		rotation = { 0, 0, 0 };
	}

	void Camera::update(GLFWwindow* window)
	{
		calculatePitch(window);
		calculateAngle(window);

		float horizontalDistance = distanceFromEntity * glm::cos(glm::radians(rotation.x));
		float verticalDistance = distanceFromEntity * glm::sin(glm::radians(rotation.x));

		float theta = entityToFollow.getRotation().y + angleAroundEntity;
		float offsetX = horizontalDistance * glm::sin(glm::radians(theta));
		float offsetZ = horizontalDistance * glm::cos(glm::radians(theta));
		
		position.x = entityToFollow.getPosition().x - offsetX;
		position.z = entityToFollow.getPosition().z - offsetZ;
		position.y = entityToFollow.getPosition().y + verticalDistance;

		rotation.y = 180 - (entityToFollow.getRotation().y + angleAroundEntity);
	}

	void Camera::calculatePitch(GLFWwindow* window)
	{
		static double preMouseY = 0.0;
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		float mouseChangeY = mouseY - preMouseY;
		preMouseY = mouseY;

		rotation.x += mouseChangeY * SENSITIVITY;
		rotation.x = toolbox::Clamp(rotation.x, -10, 90);
	}

	void Camera::calculateAngle(GLFWwindow* window)
	{
		static double preMouseX = 0.0;
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		float mouseChangeX = mouseX - preMouseX;
		preMouseX = mouseX;

		angleAroundEntity -= mouseChangeX * SENSITIVITY;
	}
}
