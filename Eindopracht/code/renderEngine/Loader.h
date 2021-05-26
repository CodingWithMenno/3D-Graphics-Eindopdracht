#pragma once

#include <string>
#include <vector>
#include "../models/Model.h"

namespace renderEngine
{
	namespace loader
	{
		/*
			This function generates a model from model data.
		*/
		models::RawModel LoadToVAO(std::vector<float>& positions, std::vector<float>& textureCoords, std::vector<float>& normals, std::vector<unsigned int>& indices);

		/*
			This function generates a model from model data.
		*/
		models::RawModel LoadToVAO(std::vector<float>& positions, int dimensions);
		
		/*
			Loads a texture from a file into openGL using stb_image.h
		 */
		GLuint LoadTexture(std::string fileName);
		
		/*
			Call this function when cleaning up all the meshes (when exiting the program).
		*/
		void CleanUp();
	}
}