// Local
#include <engine/vessel.hpp>
#include <engine/entity.hpp>
#include <engine/volume.hpp>

namespace vast::core
{
	using namespace engine;

	template<> ComponentBox<Vessel> Component<Vessel>::box = ComponentBox<Vessel>();

	template<> void Component<Vessel>::init(Scene& scene)
	{
		(void)scene;

		std::cout << "Vessel component initiated!" << std::endl;
	}

	template<> void Component<Vessel>::add(Scene& scene, id_t id)
	{
		// Add entity and volume first
		Component<engine::Entity>::add(scene, id);
		Component<engine::Volume>::add(scene, id);

		self::box.emplace(scene, id);
	}

	template<> void Component<Vessel>::tick(Scene& scene, float dt)
	{
		for (auto vessel : self::box.items(scene))
			vessel.second->tick(dt);
	}

	template<> void Component<Vessel>::remove(Scene& scene, id_t id)
	{
		self::box.remove(scene, id);
	}
}
