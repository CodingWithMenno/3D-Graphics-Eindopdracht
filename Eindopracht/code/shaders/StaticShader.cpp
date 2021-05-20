#include "StaticShader.h"
#include "../toolbox/Toolbox.h"

namespace shaders
{
	static std::string vertexShader = R"(
	#version 400 core
	// The VertexShader is run for each vertex on the screen.


	// Position of the vertex
	in vec3 position;
	// Coordinates of the texture
	in vec2 textureCoords;
	// The normal of the vertex
	in vec3 normal;

	out vec2 passTextureCoords;	
	out vec3 surfaceNormal;
	out vec3 toLightVector;

	uniform mat4 modelMatrix;
	uniform mat4 projectionMatrix;
	uniform mat4 viewMatrix;
	uniform vec3 lightPosition;


	void main(void)
	{
		// Calculate the real position of the vertex (after rotation and scaling)
		vec4 worldPosition = modelMatrix * vec4(position, 1.0);
	
		// Tell OpenGL where to render the vertex
		gl_Position = projectionMatrix * viewMatrix * worldPosition;

		// Pass the textureCoords directly to the fragment shader
		passTextureCoords = textureCoords;

		surfaceNormal = (modelMatrix * vec4(normal, 0.0)).xyz;
		toLightVector = lightPosition - worldPosition.xyz;
	}
	)";
	

	static std::string fragmentShader = R"(
	#version 400 core
	// The FragmentShader is run for each pixel in a face on the screen.


	// Interpolated textureCoordinates of the vertex (relative to the distance to each vertex)
	in vec2 passTextureCoords;

	in vec3 surfaceNormal;
	in vec3 toLightVector;

	// Final color of the pixel
	out vec4 outColor;

	// The texture of the model
	uniform sampler2D textureSampler;
	uniform vec3 lightColor;

	void main(void)
	{
		vec3 unitNormal = normalize(surfaceNormal);
		vec3 unitLightVector = normalize(toLightVector);

		float dotProduct = dot(unitNormal, unitLightVector);
		float brightness = max(dotProduct, 0.0);
		vec3 diffuse = brightness * lightColor;
	
		outColor = vec4(diffuse, 1.0) * texture(textureSampler, passTextureCoords);
	}
	)";
	
	
	StaticShader::StaticShader(): ShaderProgram(vertexShader, fragmentShader)
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

	void StaticShader::loadLight(entities::Light& light) const
	{
		loadVector(location_lightPosition, light.getPosition());
		loadVector(location_lightColor, light.getColor());
	}
	 
	void StaticShader::setAttributes() const
	{
		// Load the position VBO and textureCoords VBO from the VAO into the shader "in" variables
		setAttribute(0, "position");
		setAttribute(1, "textureCoords");
		setAttribute(2, "normal");
	}

	void StaticShader::getAllUniformLocations()
	{
		// Get the locations from the uniform variables from the shaders
		location_modelMatrix = getUniformLocation("modelMatrix");
		location_projectionMatrix = getUniformLocation("projectionMatrix");
		location_viewMatrix = getUniformLocation("viewMatrix");
		location_lightPosition = getUniformLocation("lightPosition");
		location_lightColor = getUniformLocation("lightColor");
	}
}
