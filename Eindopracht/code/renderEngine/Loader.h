#pragma once

#include <vector>
#include "../models/RawModel.h"

namespace renderEngine
{
	namespace loader
	{
		/*
			@brief: 
			Call this function to store a mesh into openGL.

			@param positions:
			A list with the coordinates of all the vertices of the mesh.

			@param indices:
			A list with all the indices of the mesh.

			@returns:
			A new RawModel representing a mesh.
		*/
		struct RawModel loadToVAO(std::vector<float>& positions, std::vector<int>& indices);

		/*
			@brief:
			Call this function when cleaning up all the meshes (when exiting the program).
		*/
		void cleanUp();
	}
}