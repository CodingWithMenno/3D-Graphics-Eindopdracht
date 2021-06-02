#include "CloudGroup.h"
#include "../renderEngine/Loader.h"
#include "../renderEngine/ObjLoader.h"
#include "../toolbox/Toolbox.h"


namespace entities
{
	CloudGroup::CloudGroup(glm::vec3 spawnPlace, glm::vec2 size, float totalClouds): totalClouds(totalClouds), center(spawnPlace), size(size)
	{
		models::RawModel cloudRawModel = renderEngine::LoadObjModel("res/Cloud.obj");
		models::ModelTexture cloudTexture = { renderEngine::loader::LoadTexture("res/Texture.png") };
		cloudTexture.reflectivity = 1;
		cloudModel = { cloudRawModel, cloudTexture };

		maxCoords = glm::vec2(center.x + (size.x / 2), center.z + (size.y / 2));

		for (int i = 0; i < totalClouds; i++)
		{
			int x = toolbox::Random(center.x - (size.x / 2), center.x + (size.x / 2));
			int z = toolbox::Random(center.z - (size.y / 2), center.z + (size.y / 2));
			clouds.push_back(std::make_shared<Entity>(cloudModel, glm::vec3(x, center.y, z), glm::vec3(0, 0, 0), modelSize));
		}
	}

	CloudGroup::~CloudGroup()
	{
		clouds.clear();
	}

	void CloudGroup::update(const double delta)
	{
		int counter = 0;
		for (auto it = clouds.begin(); it != clouds.end();)
		{
			std::shared_ptr<Entity> cloud = *it;

			cloud->increasePosition(glm::vec3(movementSpeed * delta, 0, 0));

			glm::vec3 pos = cloud->getPosition();
			if (pos.x > maxCoords.x || pos.z > maxCoords.y)
			{
				it = clouds.erase(it);
			} else
			{
				++it;
				++counter;
			}
		}
		
		if (clouds.size() < totalClouds)
		{
			int z = toolbox::Random(center.z - (size.y / 2), center.z +(size.y / 2));
			clouds.push_back(std::make_shared<Entity>(cloudModel, glm::vec3(center.x - (size.x / 2), center.y, z), glm::vec3(0, 0, 0), modelSize));
		}
	}
}
