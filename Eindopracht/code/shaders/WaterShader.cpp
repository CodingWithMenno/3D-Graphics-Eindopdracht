#include "../toolbox/Toolbox.h"
#include "WaterShader.h"

namespace shaders
{
	static std::string vertexShader = R"(
	#version 400 core

	in vec2 position;

	out vec2 textureCoords;

	uniform mat4 projectionMatrix;
	uniform mat4 viewMatrix;
	uniform mat4 modelMatrix;


	void main(void)
	{
		gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position.x, 0.0, position.y, 1.0);
		textureCoords = vec2(position.x / 2.0 + 0.5, position.y / 2.0 + 0.5);
	}
	)";


	static std::string fragmentShader = R"(
	#version 400 core

	in vec2 textureCoords;

	out vec4 outColor;


	void main(void)
	{
		outColor = vec4(0.0, 0.0, 1.0, 1.0);
	}
	)";
	
	WaterShader::WaterShader(): ShaderProgram(vertexShader, fragmentShader)
	{ }

	void WaterShader::loadModelMatrix(const glm::mat4& matrix) const
	{
		loadMatrix(location_modelMatrix, matrix);
	}

	void WaterShader::loadProjectionMatrix(const glm::mat4& projection) const
	{
		loadMatrix(location_projectionMatrix, projection);
	}

	void WaterShader::loadViewMatrix(entities::Camera& camera) const
	{
		const glm::mat4 viewMatrix = toolbox::CreateViewMatrix(camera);
		loadMatrix(location_viewMatrix, viewMatrix);
	}

	void WaterShader::setAttributes() const
	{
		setAttribute(0, "position");
	}

	void WaterShader::getAllUniformLocations()
	{
		location_modelMatrix = getUniformLocation("modelMatrix");
		location_projectionMatrix = getUniformLocation("projectionMatrix");
		location_viewMatrix = getUniformLocation("viewMatrix");
	}
}
