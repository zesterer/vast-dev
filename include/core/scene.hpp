#ifndef VAST_HPP_CORE_SCENE
#define VAST_HPP_CORE_SCENE

// Local
#include <core/input.hpp>
#include <util/result.hpp>

// Lib
#include <glm/glm.hpp>

// Std
#include <vector>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <functional>

typedef long oid_t; // An ID for each object

namespace vast::core
{
	struct Scene;

	struct ComponentCall
	{
		typedef std::function<void (Scene&)> init_t;
		typedef std::function<void (Scene&, oid_t)> add_t;
		typedef std::function<void (Scene&, oid_t)> remove_t;
		typedef std::function<void (Scene&, float)> tick_t;

		init_t init;
		add_t add;
		remove_t remove;
		tick_t tick;

		ComponentCall(
			init_t init,
			add_t add,
			remove_t remove,
			tick_t tick
		) :
			init(init),
			add(add),
			remove(remove),
			tick(tick)
		{}
	};

	struct Scene
	{
		// IDs and counters
		static int _scene_counter;
		int id = ++_scene_counter;
		oid_t _object_counter = 0;
		std::unordered_set<oid_t> objects;

		std::vector<ComponentCall> calls;

		oid_t root, player, cam;

		float time;

		oid_t new_object()
		{
			this->objects.emplace(++this->_object_counter);
			return this->_object_counter;
		}

		template <typename T> T* get(oid_t id);
		template <typename ... Args> oid_t create();
		void remove(oid_t id)
		{
			for (size_t i = this->calls.size(); i-- > 0;)
				this->calls[0].remove(*this, id);
		}

		void tick(float dt);
		void handle_inputs(InputState const& input_state);
		void setup();
		void clear();

		Scene() : time(0.0f) {}

		~Scene()
		{
			this->clear();
		}
	};

	enum class ComponentError { INVALID_ROOT, NO_SUCH_COMPONENT };
	template <typename T>
	struct ComponentBox
	{
		// A hashmap containing all components of this variant (TODO: Replace the std::map with a more performant cache-friendly structure)
		std::unordered_map<int, std::unordered_map<oid_t, std::shared_ptr<T>>> _items;

		util::Result<std::shared_ptr<T>, ComponentError> get(Scene const& scene, oid_t id)
		{
			// Have we yet registed this component root?
			auto it0 = this->_items.find(scene.id);
			if (it0 == this->_items.end())
				return util::Result<std::shared_ptr<T>, ComponentError>::failure(ComponentError::INVALID_ROOT);

			// Have we registered this id as a figure?
			auto it1 = it0->second.find(id);
			if (it1 == it0->second.end())
				return util::Result<std::shared_ptr<T>, ComponentError>::failure(ComponentError::NO_SUCH_COMPONENT);

			return util::Result<std::shared_ptr<T>, ComponentError>::success(it1->second);
		}

		template <typename ... Args>
		std::shared_ptr<T> emplace(Scene const& scene, oid_t id, Args ... args)
		{
			auto it = this->_items[scene.id].find(id);
			if (it == this->_items[scene.id].end())
			{
				auto ptr = std::make_shared<T>(args ...);
				this->_items[scene.id][id] = ptr;
				return ptr;
			}
			else
				return it->second;
		}

		void remove(Scene const& scene, oid_t id)
		{
			auto it = this->_items[scene.id].find(id);
			if (it != this->_items[scene.id].end())
				this->_items[scene.id].erase(it);
		}

		// TODO: This API SERIOUSLY needs improving, shouldn't just return map reference
		std::unordered_map<oid_t, std::shared_ptr<T>>& items(Scene const& scene)
		{
			return this->_items[scene.id];
		}
	};

	template <typename T>
	struct Component
	{
		typedef Component<T> self;

		static ComponentBox<T> box;

		// Required
		static void init(Scene& scene);
		static void add(Scene& scene, oid_t id);
		static void remove(Scene& scene, oid_t id);
		static void tick(Scene& scene, float dt);

		static ComponentCall get_calls()
		{
			return ComponentCall(
				Component<T>::init,
				Component<T>::add,
				Component<T>::remove,
				Component<T>::tick
			);
		}

		static T* get(Scene const& scene, oid_t id)
		{
			if (auto obj = self::box.get(scene, id))
				return &**obj;
			else
				return nullptr;
		}
	};

	template <typename T> T* Scene::get(oid_t id) { return Component<T>::get(*this, id); }

	// Try to ignore this horribleness
	template <typename ... Args> void _create_comp(Scene& scene, oid_t id);
	template <> void _create_comp<>(Scene& scene, oid_t id);
	template <typename T, typename ... Args>
	void _create_comp_helper(Scene& scene, oid_t id) {
		Component<T>::add(scene, id);
		return _create_comp<Args...>(scene, id);
	}
	template <typename ... Args>
	void _create_comp(Scene& scene, oid_t id) {
		_create_comp_helper<Args...>(scene, id);
	}

	template <typename ... Args> oid_t Scene::create()
	{
		oid_t nid = this->new_object();
		_create_comp<Args...>(*this, nid);
		return nid;
	}

	struct ObjIdent
	{
		oid_t id;
		Scene& scene;

		ObjIdent(oid_t id, Scene& scene) : id(id), scene(scene) {}
	};
}

#endif
