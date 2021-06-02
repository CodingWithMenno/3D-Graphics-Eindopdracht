#include "../toolbox/Toolbox.h"
#include "WaterShader.h"

namespace shaders
{
	static std::string vertexShader = R"(
	#version 400 core

	in vec2 position;

	out vec4 clipSpace;
	out vec3 toCameraVector;
	out vec3 fromLightVector;
	
	uniform mat4 projectionMatrix;
	uniform mat4 viewMatrix;
	uniform mat4 modelMatrix;
	uniform vec3 lightPosition;

	void main(void)
	{	
		vec4 worldPosition = modelMatrix * vec4(position.x, 0.0, position.y, 1.0);
	
		clipSpace = projectionMatrix * viewMatrix * worldPosition;
		gl_Position = clipSpace;

		toCameraVector = (inverse(viewMatrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz;

		fromLightVector = worldPosition.xyz - lightPosition;
	}
	)";


	static std::string fragmentShader = R"(
	#version 400 core

	in vec4 clipSpace;
	in vec3 toCameraVector;
	in vec3 fromLightVector;
	
	out vec4 outColor;

	uniform sampler2D reflectTexture;
	uniform sampler2D refractTexture;
	uniform vec3 lightColor;

	const float reflectivity = 2.0;
	const float shineDamper = 20.0;
	const float lightReflectivity = 0.6;
	
	void main(void)
	{
		// Convert to normalized device space and then to screen space
		vec2 screenSpace = (clipSpace.xy / clipSpace.w) / 2.0 + 0.5;

		vec2 reflectTexCoords = vec2(screenSpace.x, -screenSpace.y);
		vec2 refractTexCoords = vec2(screenSpace.x, screenSpace.y);
	
		vec4 reflectColor = texture(reflectTexture, reflectTexCoords);
		vec4 refractColor = texture(refractTexture, refractTexCoords);

		// Fresnel effect
		vec3 viewVector = normalize(toCameraVector);
		vec3 normal = vec3(0.0, 1.0, 0.0);
		float refractiveFactor = dot(viewVector, normal);
		refractiveFactor = pow(refractiveFactor, reflectivity);

		// Specular lighting
		vec3 reflectedLightDirection = reflect(normalize(fromLightVector), normal);
		float dotSpecular = dot(reflectedLightDirection, viewVector);
		dotSpecular = max(dotSpecular, 0.0);
		float dampedSpecular = pow(dotSpecular, shineDamper);
		vec3 specular = dampedSpecular * lightReflectivity * lightColor;
	
		outColor = mix(reflectColor, refractColor, refractiveFactor);
		outColor = mix(outColor, vec4(0.0, 0.6, 0.9, 1.0), 0.2) + vec4(specular, 0.0);
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

	void WaterShader::loadLight(const entities::Light& sun)
	{
		loadVector(location_lightColor, sun.getColor());
		loadVector(location_lightPosition, sun.getPosition());
	}

	void WaterShader::connectTextures()
	{
		loadInt(location_reflectTexture, 0);
		loadInt(location_refractTexture, 1);
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
		location_reflectTexture = getUniformLocation("reflectTexture");
		location_refractTexture = getUniformLocation("refractTexture");
		location_lightColor = getUniformLocation("lightColor");
		location_lightPosition = getUniformLocation("lightPosition");
	}
}
