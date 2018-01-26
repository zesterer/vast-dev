// Local
#include <engine/volume.hpp>

namespace vast::core
{
	using namespace engine;

	template<> ComponentBox<Volume> Component<Volume>::box = ComponentBox<Volume>();

	template<> void Component<Volume>::init(Scene& scene)
	{
		(void)scene;

		std::cout << "Volume component initiated!" << std::endl;
	}

	template<> void Component<Volume>::add(Scene& scene, id_t id)
	{
		self::box.emplace(scene, id, glm::ivec3(9, 9, 9));
	}

	template<> void Component<Volume>::tick(Scene& scene, float dt)
	{
		(void)scene;
		(void)dt;

		// Nothing yet
	}

	template<> void Component<Volume>::remove(Scene& scene, id_t id)
	{
		self::box.remove(scene, id);
	}
}
