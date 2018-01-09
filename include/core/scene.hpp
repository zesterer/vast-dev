#ifndef VAST_HPP_CORE_SCENE
#define VAST_HPP_CORE_SCENE

// Local
#include <core/cm.hpp>

// Std
#include <vector>
#include <memory>
#include <functional>

namespace vast::core
{
	struct Scene
	{
		float _time;
		id_t _id_counter;
		ComponentRoot _root;
		std::vector<ComponentType> _component_types;

		void tick(float dt)
		{
			this->_time += dt;

			for (ComponentType c : this->_component_types)
				c.tick(this->_root, dt);
		}

		void add_component_type(ComponentType c)
		{
			this->_component_types.push_back(c);
		}

		void create_object(int variant)
		{
			id_t new_id = ++this->_id_counter;

			for (ComponentType c : this->_component_types)
				c.create(this->_root, variant, new_id);
		}

		void setup()
		{
			// Create a few test objects
			for (size_t i = 0; i < 10; i ++)
				this->create_object(0);
		}

		Scene() : _time(0.0f), _id_counter(0) {}
	};
}

#endif
