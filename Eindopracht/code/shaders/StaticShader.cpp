#include "StaticShader.h"
#include "../toolbox/Toolbox.h"

namespace shaders
{
	StaticShader::StaticShader(): ShaderProgram(VERTEX_FILE, FRAGMENT_FILE)
	{
	}

	void StaticShader::loadModelMatrix(const glm::mat4& matrix) const
	{
		loadMatrix(location_modelMatrix, matrix);
	}

	void StaticShader::loadProjectionMatrix(const glm::mat4& projection) const
	{
		loadMatrix(location_projectionMatrix, projection);
	}

	void StaticShader::loadViewMatrix(entities::Camera& camera) const
	{
		const glm::mat4 viewMatrix = toolbox::createViewMatrix(camera);
		loadMatrix(location_viewMatrix, viewMatrix);
	}

	void StaticShader::setAttributes() const
	{
		setAttribute(0, "position");
		setAttribute(1, "textureCoords");
	}

	void StaticShader::getAllUniformLocations()
	{
		location_modelMatrix = getUniformLocation("modelMatrix");
		location_projectionMatrix = getUniformLocation("projectionMatrix");
		location_viewMatrix = getUniformLocation("viewMatrix");
	}
}
