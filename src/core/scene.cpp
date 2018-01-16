// Local
#include <core/scene.hpp>
#include <core/engine/entity.hpp>
#include <gfx/figure.hpp>

namespace vast::core
{
	void Scene::setup()
	{
		// Create a camera
		this->cam = this->create<engine::Entity>();

		this->create<gfx::Figure>();

		if (auto entity = this->get<engine::Entity>(this->create<gfx::Figure>()))
			entity->pos = glm::vec3(8, 8, 0);
	}

	void Scene::tick(float dt)
	{
		if (auto cam = this->get<engine::Entity>(this->cam))
		{
			float per = this->time * 0.01f;
			cam->pos = glm::vec3(-glm::cos(per), glm::sin(per), 0) * 7.0f;
			cam->ori = glm::rotate(glm::quat(), -per, glm::vec3(0, 0, 1));
		}

		this->croot.call_tick(dt);
		this->time += dt;
	}

	void Scene::clear()
	{
		// Remove all objects from the scene
		for (auto iter = this->croot.ids.begin(); iter != this->croot.ids.end(); ++iter)
			this->croot.call_remove(*iter);
	}
}
