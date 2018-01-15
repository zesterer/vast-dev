// Local
#include <core/scene.hpp>
#include <core/engine/entity.hpp>

namespace vast::core
{
	void Scene::setup(int figure_variant)
	{
		// Create a camera
		this->cam = this->create_object(engine::ENTITY_VARIANT_ID);

		for (size_t i = 0; i < 1; i ++)
			this->create_object(figure_variant);
	}

	void Scene::tick(float dt)
	{
		if (auto cam = this->get<engine::Entity>(this->cam))
		{
			float per = this->time * 0.01f;
			cam->pos = glm::vec3(-glm::cos(per), glm::sin(per), 0) * 10.0f;
			cam->ori = glm::rotate(glm::quat(), -per, glm::vec3(0, 0, 1));
		}

		this->croot.call_tick(dt);
		this->time += dt;
	}
}
