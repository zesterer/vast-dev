// Local
#include <core/engine/entity.hpp>

namespace vast::core::engine
{
	// Used to store the unique variant id for entities
	int ENTITY_VARIANT_ID;

	// A box containing all entities
	core::ComponentBox<Entity> entities;

	// Get a reference to an entity, given a component root and an object id
	util::Result<std::shared_ptr<Entity>, core::ComponentError> entity_get(ComponentRoot const& root, id_t id)
	{
		return entities.get(root, id);
	}

	// Initiate the figure variant
	void entity_init(core::ComponentRoot& root)
	{
		(void)root;
		// Nothing yet
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

	// Perform a tick on all entity components
	void entity_tick(ComponentRoot& root, float dt)
	{
		for (auto pair : entities.components(root))
			pair.second->tick(dt);
	}

	// Create an instance describing the entity variant
	ComponentVariant entity_variant()
	{
		return ComponentVariant(
			ENTITY_VARIANT_ID,
			&entity_init,
			&entity_create,
			&entity_remove,
			&entity_tick
		);
	}

	// Register the entity as a component variant
	__attribute__((constructor)) void register_entity_var()
	{
		ENTITY_VARIANT_ID = cm_register_component();
	}
}
