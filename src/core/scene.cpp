// Local
#include <core/scene.hpp>
#include <engine/entity.hpp>
#include <engine/volume.hpp>
#include <engine/vessel.hpp>
#include <gfx/figure.hpp>
#include <gfx/body.hpp>

#include <stdlib.h>

namespace vast::core
{
	int Scene::_scene_counter = 0;

	// To fix some horribleness in scene.hpp
	template <> void _create_comp<>(Scene& scene, id_t id) { (void)scene; (void)id; }

	void Scene::setup()
	{
		for (auto call : this->calls)
			call.init(*this);

		// Create a camera
		this->cam = this->create<engine::Entity>();

		this->create<gfx::Figure>();

		if (auto entity = this->get<engine::Entity>(this->create<gfx::Body, engine::Vessel>()))
			entity->pos = glm::vec3(8, 8, 0);
	}

	void Scene::tick(float dt)
	{
		if (auto cam = this->get<engine::Entity>(this->cam))
		{
			float per = this->time * 0.01f;
			cam->pos = glm::vec3(-glm::cos(per), glm::sin(per), 0) * 8.0f + glm::vec3(0, 0, 3);
			cam->ori = glm::rotate(glm::quat(), -per, glm::vec3(0, 0, 1));
		}

		for (auto call : this->calls)
			call.tick(*this, dt);

		//this->croot.call_tick(dt);
		this->time += dt;
	}

	void Scene::clear()
	{
		// Remove all objects from the scene
		for (auto id : this->objects)
			this->remove(id);
	}
}
