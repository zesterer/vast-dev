#ifndef VAST_HPP_CORE_CM
#define VAST_HPP_CORE_CM

// Std
#include <functional>

namespace vast::core
{
	typedef size_t cm_id;
	typedef size_t id_t;

	struct ComponentRoot;

	struct ComponentType
	{
		std::function<void (ComponentRoot&, int, id_t)> create;
		std::function<void (ComponentRoot&, id_t)> remove;
		std::function<void (ComponentRoot&, float)> tick;

		ComponentType(
			std::function<void (ComponentRoot&, int, id_t)> create,
			std::function<void (ComponentRoot&, id_t)> remove,
			std::function<void (ComponentRoot&, float)> tick
		);
	};

	struct ComponentRoot
	{
		cm_id id;
		id_t _entity_id_counter;
		std::vector<ComponentType> _types;

		void add_type(ComponentType type)
		{
			this->_types.push_back(type);
		}

		id_t new_entity_id()
		{
			return ++this->_entity_id_counter;
		}

		void call_create(int variant, id_t id)
		{
			// Create object components (in order of component precedence)
			for (ComponentType c : this->_types)
				c.create(*this, variant, id);
		}

		void call_tick(float dt)
		{
			// Tick components
			for (ComponentType c : this->_types)
				c.tick(*this, dt);
		}

		void call_remove(id_t id)
		{
			// Remove object components (in reverse order of component precedence)
			for (
				std::vector<ComponentType>::reverse_iterator it = this->_types.rbegin();
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
