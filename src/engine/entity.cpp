// Local
#include <engine/entity.hpp>

namespace vast::core
{
	using namespace engine;

	template<> ComponentBox<Entity> Component<Entity>::box = ComponentBox<Entity>();

	template<> void Component<Entity>::init(Scene& scene)
	{
		(void)scene;

		std::cout << "Entity component initiated!" << std::endl;
	}

	template<> void Component<Entity>::add(Scene& scene, id_t id)
	{
		self::box.emplace(scene, id);
	}

	template<> void Component<Entity>::tick(Scene& scene, float dt)
	{
		for (auto entity : self::box.items(scene))
			entity.second->tick(dt);
	}

	template<> void Component<Entity>::remove(Scene& scene, id_t id)
	{
		self::box.remove(scene, id);
	}
}
