#ifndef VAST_HPP_CORE_ENTITY
#define VAST_HPP_CORE_ENTITY

// Lib
#include <glm/glm.hpp>

// Local
#include <core/cm.hpp>
#include <util/result.hpp>

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

	int entity_variant;
	std::map<cm_id, std::map<id_t, Entity>> entities;

	enum class EntityError { INVALID_ROOT, NOT_AN_ENTITY };

	// Get a reference to an entity, given a component root and an object id
	util::Result<Entity&, EntityError> entity_get(ComponentRoot& root, id_t id)
	{
		auto it0 = entities.find(root.id);
		if (it0 == entities.end())
			return util::Result<Entity&, EntityError>::failure(EntityError::INVALID_ROOT);

		auto it1 = it0->second.find(id);
		if (it1 == it0->second.end())
			return util::Result<Entity&, EntityError>::failure(EntityError::NOT_AN_ENTITY);

		return util::Result<Entity&, EntityError>::success(it1->second);
	}

	// Register the entity as a component variant
	__attribute__((constructor)) void register_entity()
	{
		entity_variant = cm_register_component();
	}

	// Create a new entity component
	void entity_create(ComponentRoot& root, int variant, id_t id)
	{
		if (variant != entity_variant)
			return;
		
		entities[root.id].emplace(id, Entity());
		std::cout << "Created entity!" << std::endl;
	}

	// Perform a tick on an entity component
	void entity_tick(ComponentRoot& root, float dt)
	{
		for (auto pair : entities[root.id])
		{
			Entity& entity = pair.second;

			entity.tick(dt);
		}
	}

	// Create a new entity component type
	ComponentType entity_component_type()
	{
		return ComponentType(&entity_create, &entity_tick);
	}
}

#endif
