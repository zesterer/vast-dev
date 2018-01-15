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
		float time;
		ComponentRoot croot;
		std::vector<id_t> _objects;

		id_t camera;

		std::vector<id_t> const& get_objects() const
		{
			return this->_objects;
		}

		ComponentRoot const& get_croot() const { return this->croot; }
		ComponentRoot& get_croot() { return this->croot; }

		void tick(float dt)
		{
			this->croot.call_tick(dt);
			this->time += dt;
		}

		id_t create_object(int variant)
		{
			id_t new_id = this->croot.new_entity_id();
			this->croot.call_create(variant, new_id);

			this->_objects.push_back(new_id);
			return new_id;
		}

		void remove_object(id_t id)
		{
			this->croot.call_remove(id);
		}

		void setup(int figure_variant)
		{
			// Create a few test objects
			for (size_t i = 0; i < 1; i ++)
				this->create_object(figure_variant);
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

		Scene() : time(0.0f) {}

		~Scene()
		{
			this->clear();
		}
	};
}

#endif
