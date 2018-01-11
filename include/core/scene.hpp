#ifndef VAST_HPP_CORE_SCENE
#define VAST_HPP_CORE_SCENE

// Local
#include <core/cm.hpp>
#include <core/engine/entity.hpp>

// Std
#include <vector>
#include <memory>
#include <functional>

namespace vast::core
{
	struct Scene
	{
		float _time;
		ComponentRoot cr;
		std::vector<id_t> _objects;

		void tick(float dt)
		{
			this->cr.call_tick(dt);
			this->_time += dt;
		}

		id_t create_object(int variant)
		{
			id_t new_id = this->cr.new_entity_id();
			this->cr.call_create(variant, new_id);

			this->_objects.push_back(new_id);
			return new_id;
		}

		void remove_object(id_t id)
		{
			this->cr.call_remove(id);
		}

		void setup()
		{
			// Create a few test objects
			for (size_t i = 0; i < 10; i ++)
				this->create_object(engine::ENTITY_VARIANT_ID);
		}

		void clear()
		{
			// Remove all objects from the scene
			while (this->_objects.size() > 0)
			{
				this->remove_object(this->_objects.back());
				this->_objects.pop_back();
			}
		}

		Scene() : _time(0.0f) {}

		~Scene()
		{
			this->clear();
		}
	};
}

#endif
