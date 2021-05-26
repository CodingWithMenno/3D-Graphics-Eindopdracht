#pragma once

#include <string>
#include "../models/Model.h"

namespace renderEngine
{
	models::RawModel LoadObjModel(std::string fileName);
}