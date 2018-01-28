// Local
#include <engine/agent.hpp>
#include <engine/entity.hpp>

namespace vast::core
{
	using namespace engine;

	template<> ComponentBox<Agent> Component<Agent>::box = ComponentBox<Agent>();

	template<> void Component<Agent>::init(Scene& scene)
	{
		(void)scene;

		std::cout << "Agent component initiated!" << std::endl;
	}

	template<> void Component<Agent>::add(Scene& scene, id_t id)
	{
		// Add entity first
		Component<engine::Entity>::add(scene, id);

		self::box.emplace(scene, id);
	}

	template<> void Component<Agent>::tick(Scene& scene, float dt)
	{
		for (auto agent : self::box.items(scene))
			agent.second->tick(dt);
	}

	template<> void Component<Agent>::remove(Scene& scene, id_t id)
	{
		self::box.remove(scene, id);
	}
}
