#ifndef VAST_HPP_CORE_SCENE
#define VAST_HPP_CORE_SCENE

// Local
#include <util/result.hpp>

// Lib
#include <glm/glm.hpp>

// Std
#include <vector>
#include <memory>
#include <unordered_set>
#include <functional>

namespace vast::core
{
	typedef size_t id_t; // An ID for each object

	struct Scene;

	struct ComponentCall
	{
		typedef std::function<void (Scene&)> init_t;
		typedef std::function<void (Scene&, id_t)> add_t;
		typedef std::function<void (Scene&, id_t)> remove_t;
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
		id_t _object_counter = 0;
		std::unordered_set<id_t> objects;

		std::vector<ComponentCall> calls;

		id_t cam;

		float time;

		id_t new_object()
		{
			this->objects.emplace(++this->_object_counter);
			return this->_object_counter;
		}

		template <typename T> T* get(id_t id);
		template <typename T> id_t create();
		void remove(id_t id)
		{
			for (size_t i = this->calls.size(); i-- > 0;)
				this->calls[0].remove(*this, id);
		}

		void tick(float dt);
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
		std::unordered_map<int, std::unordered_map<id_t, std::shared_ptr<T>>> _items;

		util::Result<std::shared_ptr<T>, ComponentError> get(Scene const& scene, id_t id)
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
		bool emplace(Scene const& scene, id_t id, Args ... args)
		{
			auto it = this->_items[scene.id].find(id);
			if (it == this->_items[scene.id].end())
			{
				this->_items[scene.id][id] = std::make_shared<T>(args ...);
				return true;
			}
			else
				return false;
		}

		void remove(Scene const& scene, id_t id)
		{
			auto it = this->_items[scene.id].find(id);
			if (it != this->_items[scene.id].end())
				this->_items[scene.id].erase(it);
		}

		// TODO: This API SERIOUSLY needs improving, shouldn't just return map reference
		std::unordered_map<id_t, std::shared_ptr<T>>& items(Scene const& scene)
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
		static void add(Scene& scene, id_t id);
		static void remove(Scene& scene, id_t id);
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

		static T* get(Scene const& scene, id_t id)
		{
			if (auto obj = self::box.get(scene, id))
				return &**obj;
			else
				return nullptr;
		}
	};

	template <typename T> T* Scene::get(id_t id) { return Component<T>::get(*this, id); }
	template <typename T> id_t Scene::create()
	{
		id_t nid = this->new_object();
		Component<T>::add(*this, nid);
		return nid;
	}
}

#endif
