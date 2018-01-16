// Local
#include <core/engine/entity.hpp>
#include <core/scene.hpp>

namespace vast::core::engine
{
	// Used to store the unique variant id for entities
	int ENTITY_VARIANT_ID;

	// A box containing all entities
	core::ComponentBox<Entity> entities;

	// Initiate the figure variant
	void entity_init(core::ComponentRoot& root)
	{
		(void)root;
		// Nothing yet
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

namespace vast::core
{
	using namespace engine;

	template <> Entity* Scene::get<Entity>(id_t id)
	{
		auto entity = entities.get(this->croot, id);
		if (entity)
			return &**entity;
		else
			return nullptr;
	}

	template <> id_t Scene::create<engine::Entity>()
	{
		id_t id = this->croot.new_object();

		entities.emplace(this->croot, id);

		return id;
	}
}
