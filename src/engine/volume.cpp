// Local
#include <engine/volume.hpp>

#include <cstdio>

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
		auto volume = self::box.emplace(scene, id, glm::ivec3(9, 9, 9));

		// Randomise volume
		for (int i = 0; i < volume->size[0]; i ++)
			for (int j = 0; j < volume->size[1]; j ++)
				for (int k = 0; k < volume->size[2]; k ++)
					volume->set(i, j, k, Voxel(std::rand() % 2));
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
