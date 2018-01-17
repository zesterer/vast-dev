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

	template<> id_t Component<Volume>::create(Scene& scene)
	{
		id_t nid = scene.new_object();
		self::box.emplace(scene, nid, glm::ivec3(9, 9, 9));
		return nid;
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
