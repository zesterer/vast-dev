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

	template<> void Component<Entity>::add(Scene& scene, oid_t id)
	{
		self::box.emplace(scene, id, ObjIdent(id, scene));
	}

	template<> void Component<Entity>::tick(Scene& scene, float dt)
	{
		if (auto entity = self::get(scene, scene.root))
			entity->tick(glm::mat4(1), dt);
	}

	template<> void Component<Entity>::remove(Scene& scene, oid_t id)
	{
		self::box.remove(scene, id);
	}
}
