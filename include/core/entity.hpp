#ifndef VAST_HPP_CORE_ENTITY
#define VAST_HPP_CORE_ENTITY

// Lib
#include <glm/glm.hpp>

// Local
#include <core/cm.hpp>

// Std
#include <iostream>
#include <map>
#include <vector>

namespace vast::core
{
	struct Entity
	{
		glm::vec3 pos;
		glm::vec3 vel;

		void tick(float dt)
		{
			this->pos += this->vel * dt;
		}
	};

	std::map<cm_id, std::map<id_t, Entity>> entities;

	void entity_create(ComponentRoot& root, int variant, id_t id)
	{
		entities[root.id].emplace(id, Entity());
		std::cout << "Created entity!" << std::endl;
	}

	void entity_tick(ComponentRoot& root, float dt)
	{
		for (auto pair : entities[root.id])
		{
			Entity& entity = pair.second;

			entity.tick(dt);
		}
	}

	Component create_entity_component()
	{
		return Component(&entity_create, &entity_tick);
	}
}

#endif
