#ifndef VAST_HPP_CORE_CM
#define VAST_HPP_CORE_CM

// Local
#include <util/result.hpp>

// Std
#include <functional>
#include <map>
#include <memory>

namespace vast::core
{
	typedef size_t cm_id; // Component ID (an ID for ComponentRoot objects)
	typedef size_t id_t; // An ID for each object

	struct ComponentRoot;

	// ComponentVariant describes a component variant and its functions
	struct ComponentVariant
	{
		typedef std::function<void (ComponentRoot&)> init_func_t;
		typedef std::function<void (ComponentRoot&, id_t)> create_func_t;
		typedef std::function<void (ComponentRoot&, id_t)> remove_func_t;
		typedef std::function<void (ComponentRoot&, float)> tick_func_t;

		int variant_id;
		init_func_t init;
		create_func_t create;
		remove_func_t remove;
		tick_func_t tick;

		ComponentVariant(
			int variant_id,
			init_func_t init,
			create_func_t create,
			remove_func_t remove,
			tick_func_t tick
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
			type.init(*this); // Init the variant
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

		// Tick all components in this root
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

	enum class ComponentError { INVALID_ROOT, NO_SUCH_COMPONENT };
	template <typename T>
	struct ComponentBox
	{
		// A hashmap containing all components of this variant (TODO: Replace the std::map with a more performant cache-friendly structure)
		std::map<cm_id, std::map<id_t, std::shared_ptr<T>>> _items;

		util::Result<std::shared_ptr<T>, ComponentError> get(ComponentRoot const& root, id_t id)
		{
			// Have we yet registed this component root?
			auto it0 = this->_items.find(root.id);
			if (it0 == this->_items.end())
				return util::Result<std::shared_ptr<T>, ComponentError>::failure(ComponentError::INVALID_ROOT);

			// Have we registered this id as a figure?
			auto it1 = it0->second.find(id);
			if (it1 == it0->second.end())
				return util::Result<std::shared_ptr<T>, ComponentError>::failure(ComponentError::NO_SUCH_COMPONENT);

			return util::Result<std::shared_ptr<T>, ComponentError>::success(it1->second);
		}

		template <typename ... Args>
		bool emplace(core::ComponentRoot const& root, id_t id, Args ... args)
		{
			auto it = this->_items[root.id].find(root.id);
			if (it == this->_items[root.id].end())
			{
				this->_items[root.id][id] = std::make_shared<T>(args ...);
				return true;
			}
			else
				return false;
		}

		void remove(ComponentRoot const& root, id_t id)
		{
			auto it = this->_items[root.id].find(id);
			if (it != this->_items[root.id].end())
				this->_items[root.id].erase(it);
		}

		// TODO: This API SERIOUSLY needs improving, shouldn't just return map reference
		std::map<id_t, std::shared_ptr<T>>& components(ComponentRoot const& root)
		{
			return this->_items[root.id];
		}
	};

	int cm_register_component();
}

#endif
