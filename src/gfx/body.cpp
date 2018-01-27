// Local
#include <gfx/body.hpp>

// Std
#include <functional>

namespace vast::core
{
	using namespace gfx;

	template<> ComponentBox<Body> Component<Body>::box = ComponentBox<Body>();

	template<> void Component<Body>::init(Scene& scene)
	{
		(void)scene;

		std::cout << "Body component initiated!" << std::endl;
	}

	template<> void Component<Body>::add(Scene& scene, id_t id)
	{
		// Add a volume and figure first
		Component<engine::Volume>::add(scene, id);
		Component<Figure>::add(scene, id);

		if (auto volume = scene.get<engine::Volume>(id))
			self::box.emplace(scene, id, *volume);
	}

	template<> void Component<Body>::tick(Scene& scene, float dt)
	{
		(void)dt;

		for (auto body : self::box.items(scene))
			if (auto volume = scene.get<engine::Volume>(body.first))
				if (auto figure = scene.get<Figure>(body.first))
					body.second->check_remesh(*volume, *figure);
	}

	template<> void Component<Body>::remove(Scene& scene, id_t id)
	{
		self::box.remove(scene, id);
	}
}
