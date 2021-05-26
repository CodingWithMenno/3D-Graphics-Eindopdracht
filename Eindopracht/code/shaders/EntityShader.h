#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include "ShaderProgram.h"
#include "../entities/Camera.h"
#include "../entities/Light.h"

/*
	This class does represents the shaders for the models.
 */

namespace shaders
{	
	class EntityShader : public ShaderProgram
	{
	private:
		GLuint location_modelMatrix;
		GLuint location_projectionMatrix;
		GLuint location_viewMatrix;
		GLuint location_lightPosition;
		GLuint location_lightColor;
		GLuint location_shineDamper;
		GLuint location_reflectivity;
		GLuint location_skyColor;
		
	public:
		EntityShader();

		void loadModelMatrix(const glm::mat4& matrix) const;
		void loadProjectionMatrix(const glm::mat4& projection) const;
		void loadViewMatrix(entities::Camera& camera) const;

		void loadLight(entities::Light& light) const;
		void loadShineVariables(float shineDamper, float reflectivity) const;

		void loadSkyColor(glm::vec3 color) const;
			
	protected:
		void setAttributes() const override;
		void getAllUniformLocations() override;
	};
}