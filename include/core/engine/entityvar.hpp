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

		Entity() { std::cout << "Created Entity" << std::endl; }
		~Entity() { std::cout << "Deleted Entity" << std::endl; }
	};

	// Used to store the unique variant id for entities
	static int ENTITY_VARIANT_ID;

	// A box containing all entities
	core::ComponentBox<Entity> entities;

	// Get a reference to an entity, given a component root and an object id
	util::Result<Entity&, core::ComponentBox<Entity>::Error> entity_get(ComponentRoot& root, id_t id)
	{
		return entities.get(root, id);
	}

	// Create a new entity component
	void entity_create(ComponentRoot& root, id_t id)
	{
		entities.emplace(root, id);
	}

	// Remove an entity component
	void entity_remove(ComponentRoot& root, id_t id)
	{
		entities.remove(root, id);
	}

	// Perform a tick on an entity component
	void entity_tick(ComponentRoot& root, float dt)
	{
		for (auto pair : entities.components(root))
		{
			Entity& entity = *pair.second;

			entity.tick(dt);
		}
	}

	// Register the entity as a component variant
	__attribute__((constructor)) void register_entity_var()
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
