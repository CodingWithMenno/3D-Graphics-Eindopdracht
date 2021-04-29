#pragma once

#include "../models/RawModel.h"

namespace renderEngine
{
	namespace renderer
	{
		/*
			@brief:
			Call this function before rendering. 
		*/
		void prepare();

		/*
			@brief:
			Call this function when wanting to render a mesh to the screen.

			@param model:
			A mesh.
		*/
		void render(RawModel model);
	}
}