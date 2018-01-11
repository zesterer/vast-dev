#ifndef VAST_HPP_CORE_CM
#define VAST_HPP_CORE_CM

// Std
#include <functional>

namespace vast::core
{
	typedef size_t cm_id; // Component ID (an ID for ComponentRoot objects)
	typedef size_t id_t; // An ID for each object

	struct ComponentRoot;

	// ComponentVariant describes a component variant and its functions
	struct ComponentVariant
	{
		int variant_id;
		std::function<void (ComponentRoot&, id_t)> create;
		std::function<void (ComponentRoot&, id_t)> remove;
		std::function<void (ComponentRoot&, float)> tick;

		ComponentVariant(
			int variant_id,
			std::function<void (ComponentRoot&, id_t)> create,
			std::function<void (ComponentRoot&, id_t)> remove,
			std::function<void (ComponentRoot&, float)> tick
		);
	};

	// ComponentRoot describes a class of objects containing multiple variants
	struct ComponentRoot
	{
		cm_id id;
		id_t _entity_id_counter;
		std::vector<ComponentVariant> _types;

		// Add a component variant to this class of objects
		void add_variant(ComponentVariant type)
		{
			this->_types.push_back(type);
		}

		// Generate a new unique entity id
		id_t new_entity_id()
		{
			return ++this->_entity_id_counter;
		}

		// Create a new object of a specific variant id
		void call_create(int variant, id_t id)
		{
			// Create object components (in order of component precedence)
			for (ComponentVariant c : this->_types)
				if (c.variant_id == variant)
					c.create(*this, id);
		}

		//
		void call_tick(float dt)
		{
			// Tick components
			for (ComponentVariant c : this->_types)
				c.tick(*this, dt);
		}

		void call_remove(id_t id)
		{
			// Remove object components (in reverse order of component precedence)
			for (
				std::vector<ComponentVariant>::reverse_iterator it = this->_types.rbegin();
				it != this->_types.rend();
				++it
			 )
				it->remove(*this, id);
		}

		ComponentRoot() : id(0) {}
	};

	int cm_register_component();
}

#endif
