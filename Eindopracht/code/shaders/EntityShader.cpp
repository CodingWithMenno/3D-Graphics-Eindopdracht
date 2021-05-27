#include "EntityShader.h"
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
	out vec3 toCameraVector;
	out float visibility;

	uniform mat4 modelMatrix;
	uniform mat4 projectionMatrix;
	uniform mat4 viewMatrix;
	uniform vec3 lightPosition;

	// Clipping plane variable (for the water)
	uniform vec4 plane;
	
	// Fog variables
	const float density = 0.0045;
	const float gradient = 4.0;
	
	void main(void)
	{
		// Calculate the real position of the vertex (after rotation and scaling)
		vec4 worldPosition = modelMatrix * vec4(position, 1.0);

		gl_ClipDistance[0] = dot(worldPosition, plane);
	
		vec4 positionRelToCam = viewMatrix * worldPosition;
	
		// Tell OpenGL where to render the vertex
		gl_Position = projectionMatrix * positionRelToCam;

		// Pass the textureCoords directly to the fragment shader
		passTextureCoords = textureCoords;

		surfaceNormal = (modelMatrix * vec4(normal, 0.0)).xyz;
		toLightVector = lightPosition - worldPosition.xyz;
		toCameraVector = (inverse(viewMatrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz;

		float distance = length(positionRelToCam.xyz);
		visibility = exp(-pow((distance * density), gradient));
		visibility = clamp(visibility, 0.0, 1.0);
	}
	)";
	

	static std::string fragmentShader = R"(
	#version 400 core
	// The FragmentShader is run for each pixel in a face on the screen.


	// Interpolated textureCoordinates of the vertex (relative to the distance to each vertex)
	in vec2 passTextureCoords;

	in vec3 surfaceNormal;
	in vec3 toLightVector;
	in vec3 toCameraVector;
	in float visibility;

	// Final color of the pixel
	out vec4 outColor;

	// The texture of the model
	uniform sampler2D textureSampler;
	
	uniform vec3 lightColor;
	uniform float shineDamper;
	uniform float reflectivity;
	uniform vec3 skyColor;

	void main(void)
	{
		vec3 unitNormal = normalize(surfaceNormal);
		vec3 unitLightVector = normalize(toLightVector);
		vec3 unitCameraVector = normalize(toCameraVector); 

		// Calculate the diffuse lighting
		float dotDiffuse = dot(unitNormal, unitLightVector);
		float brightness = max(dotDiffuse, 0.1);
		vec3 diffuse = brightness * lightColor;

		// Calculate the specular lighting
		vec3 lightDirection = -unitLightVector;
		vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);
		float dotSpecular = dot(reflectedLightDirection, unitCameraVector);
		dotSpecular = max(dotSpecular, 0.0);
		float dampedSpecular = pow(dotSpecular, shineDamper);
		vec3 specular = dampedSpecular * reflectivity * lightColor;
		
	
		outColor = vec4(diffuse, 1.0) * texture(textureSampler, passTextureCoords) + vec4(specular, 1.0);
		outColor = mix(vec4(skyColor, 1.0), outColor, visibility);
	}
	)";
	
	
	EntityShader::EntityShader(): ShaderProgram(vertexShader, fragmentShader)
	{ }

	void EntityShader::loadModelMatrix(const glm::mat4& matrix) const
	{
		loadMatrix(location_modelMatrix, matrix);
	}

	void EntityShader::loadProjectionMatrix(const glm::mat4& projection) const
	{
		loadMatrix(location_projectionMatrix, projection);
	}

	void EntityShader::loadViewMatrix(entities::Camera& camera) const
	{
		const glm::mat4 viewMatrix = toolbox::CreateViewMatrix(camera);
		loadMatrix(location_viewMatrix, viewMatrix);
	}

	void EntityShader::loadLight(entities::Light& light) const
	{
		loadVector(location_lightPosition, light.getPosition());
		loadVector(location_lightColor, light.getColor());
	}

	void EntityShader::loadShineVariables(float shineDamper, float reflectivity) const
	{
		loadFloat(location_shineDamper, shineDamper);
		loadFloat(location_reflectivity, reflectivity);
	}

	void EntityShader::loadSkyColor(glm::vec3 color) const
	{
		loadVector(location_skyColor, color);
	}

	void EntityShader::loadClippingPlane(glm::vec4 plane) const
	{
		loadVector(location_plane, plane);
	}

	void EntityShader::setAttributes() const
	{
		// Load the position VBO and textureCoords VBO from the VAO into the shader "in" variables
		setAttribute(0, "position");
		setAttribute(1, "textureCoords");
		setAttribute(2, "normal");
	}

	void EntityShader::getAllUniformLocations()
	{
		// Get the locations from the uniform variables from the shaders
		location_modelMatrix = getUniformLocation("modelMatrix");
		location_projectionMatrix = getUniformLocation("projectionMatrix");
		location_viewMatrix = getUniformLocation("viewMatrix");
		location_lightPosition = getUniformLocation("lightPosition");
		location_lightColor = getUniformLocation("lightColor");
		location_shineDamper = getUniformLocation("shineDamper");
		location_reflectivity = getUniformLocation("reflectivity");
		location_skyColor = getUniformLocation("skyColor");
		location_plane = getUniformLocation("plane");
	}
}
