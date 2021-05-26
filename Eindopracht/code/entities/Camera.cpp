#include "Camera.h"

namespace entities
{
	Camera::Camera(const ::glm::vec3& position, const ::glm::vec3& rotation)
		: position(position),
		rotation(rotation)
	{}

	void Camera::move(GLFWwindow* window, const double delta)
	{

	}
}
