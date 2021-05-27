#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include "ShaderProgram.h"
#include "../entities/Camera.h"

namespace shaders
{
	class WaterShader : public ShaderProgram
	{
	private:
		GLuint location_modelMatrix;
		GLuint location_projectionMatrix;
		GLuint location_viewMatrix;
		GLuint location_reflectTexture;
		GLuint location_refractTexture;

	public:
		WaterShader();

		void loadModelMatrix(const glm::mat4& matrix) const;
		void loadProjectionMatrix(const glm::mat4& projection) const;
		void loadViewMatrix(entities::Camera& camera) const;

		void connectTextures();
	
	protected:
		void setAttributes() const override;
		void getAllUniformLocations() override;
	};
}