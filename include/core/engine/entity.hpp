#ifndef VAST_HPP_CORE_ENGINE_ENTITY
#define VAST_HPP_CORE_ENGINE_ENTITY

// Lib
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

// Local
#include <core/cm.hpp>
#include <util/result.hpp>

// Std
#include <iostream>
#include <map>
#include <vector>

namespace vast::core::engine
{
	// A structure defining the layout and features of the entity component
	struct Entity
	{
		glm::vec3 pos, vel;
		glm::quat ori, rot;

		void tick(float dt)
		{
			this->pos += this->vel * dt; // Apply velocity
			this->ori = (this->rot * dt) * this->ori; // Apply rotation
		}
	};

	// Used to store the unique variant id for entities
	static int ENTITY_VARIANT_ID;

	// A hashmap containing all components of the entity variant (TODO: Replace the std::map with a more performant cache-friendly structure)
	std::map<cm_id, std::map<id_t, Entity>> entities;

	// Possible errors when attempting to manipulate entities
	enum class EntityError { INVALID_ROOT, NOT_AN_ENTITY };

	// Get a reference to an entity, given a component root and an object id
	util::Result<Entity&, EntityError> entity_get(ComponentRoot& root, id_t id)
	{
		// Have we yet registed this component root?
		auto it0 = entities.find(root.id);
		if (it0 == entities.end())
			return util::Result<Entity&, EntityError>::failure(EntityError::INVALID_ROOT);

		// Have we registered this id as an entity?
		auto it1 = it0->second.find(id);
		if (it1 == it0->second.end())
			return util::Result<Entity&, EntityError>::failure(EntityError::NOT_AN_ENTITY);

		return util::Result<Entity&, EntityError>::success(it1->second);
	}

	// Create a new entity component
	void entity_create(ComponentRoot& root, id_t id)
	{
		entities[root.id].emplace(id, Entity());
		std::cout << "Created entity!" << std::endl;
	}

	// Remove an entity component
	void entity_remove(ComponentRoot& root, id_t id)
	{
		auto it = entities[root.id].find(id);
		if (it != entities[root.id].end())
		{
			entities[root.id].erase(it);
			std::cout << "Removed entity!" << std::endl;
		}
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

	// Register the entity as a component variant
	__attribute__((constructor)) void register_entity()
	{
		ENTITY_VARIANT_ID = cm_register_component();
	}

	// Create an instance describing the entity variant
	ComponentVariant entity_variant()
	{
		return ComponentVariant(
			ENTITY_VARIANT_ID,
			&entity_create,
			&entity_remove,
			&entity_tick
		);
	}
}

#endif
