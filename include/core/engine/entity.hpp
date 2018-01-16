#ifndef VAST_HPP_CORE_ENGINE_ENTITY
#define VAST_HPP_CORE_ENGINE_ENTITY

// Lib
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

// Local
#include <core/cm.hpp>
#include <core/scene.hpp>
#include <util/result.hpp>

// Std
#include <iostream>
#include <map>

namespace vast::core::engine
{
	// A structure defining the layout and features of the entity component
	struct Entity
	{
		glm::vec3 pos, vel, scale = glm::vec3(1);
		glm::quat ori, rot;

		void tick(float dt)
		{
			glm::normalize(this->ori);
			glm::normalize(this->rot);

			this->pos += this->vel * dt; // Apply velocity
			this->ori = (this->rot * dt) * this->ori; // Apply rotation
		}

		Entity() { std::cout << "Created Entity" << std::endl; }
		~Entity() { std::cout << "Deleted Entity" << std::endl; }
	};

	// Used to store the unique variant id for entities
	extern int ENTITY_VARIANT_ID;

	// A box containing all entities
	extern core::ComponentBox<Entity> entities;

	// Remove an entity component
	void entity_remove(ComponentRoot& root, id_t id);

	// Perform a tick on all entity components
	void entity_tick(ComponentRoot& root, float dt);

	// Create an instance describing the entity variant
	ComponentVariant entity_variant();
}

namespace vast::core
{
	template <> engine::Entity* Scene::get<engine::Entity>(id_t id);
	template <> id_t Scene::create<engine::Entity>();
}

#endif
