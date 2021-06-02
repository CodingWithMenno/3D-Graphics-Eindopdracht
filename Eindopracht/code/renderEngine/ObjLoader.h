#pragma once

#include <string>
#include "../models/Model.h"

namespace renderEngine
{
	/*
	 * This function loads a .obj model from a given file into openGL and returns a RawModel
	 */
	models::RawModel LoadObjModel(std::string fileName);
}