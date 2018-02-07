// Local
#include <core/scene.hpp>
#include <engine/entity.hpp>
#include <engine/volume.hpp>
#include <engine/vessel.hpp>
#include <engine/agent.hpp>
#include <gfx/figure.hpp>
#include <gfx/body.hpp>

#include <stdlib.h>

namespace vast::core
{
	int Scene::_scene_counter = 0;

	// To fix some horribleness in scene.hpp
	template <> void _create_comp<>(Scene& scene, oid_t id) { (void)scene; (void)id; }

	void Scene::setup()
	{
		for (auto call : this->calls)
			call.init(*this);

		// Create root object
		this->root = this->create<engine::Entity>();

		oid_t spacecraft_id;
		if (auto spacecraft = this->get<engine::Entity>(spacecraft_id = this->create<gfx::Body, engine::Vessel>()))
			spacecraft->set_parent(*this, spacecraft_id, this->root);

		for (size_t i = 0; i < 5; i ++)
			if (auto spacecraft = this->get<engine::Entity>(spacecraft_id = this->create<gfx::Body, engine::Vessel>()))
			{
				spacecraft->set_parent(*this, spacecraft_id, this->root);
				spacecraft->pos = glm::vec3(rand() % 300 - 150, rand() % 300 - 150, rand() % 30 - 15);
			}

		// Create player object
		if (auto player = this->get<engine::Entity>(this->player = this->create<engine::Agent>()))
		{
			player->pos = glm::vec3(4, 0, -2);
			player->set_parent(*this, this->player, spacecraft_id);
		}
		if (auto player = this->get<engine::Agent>(this->player))
			player->size = glm::vec3(0.5, 0.5, 1.8);

		// Create camera object
		if (auto cam = this->get<engine::Entity>(this->cam = this->create<engine::Entity>()))
		{
			cam->pos.z = 1.5f;
			cam->set_parent(*this, this->cam, this->player);
		}
	}

	void Scene::tick(float dt)
	{
		for (auto call : this->calls)
			call.tick(*this, dt);

		//this->croot.call_tick(dt);
		this->time += dt;
	}

	void Scene::handle_inputs(InputState const& input_state)
	{
		if (auto player_entity = this->get<engine::Entity>(this->player))
			if (auto player_agent = this->get<engine::Agent>(this->player))
			{
				// Adjust player head angle
				player_agent->head_angle += input_state.look_offset * 0.002f;

				float yaw = player_agent->head_angle.x;

				// Alter player orientation
				if (auto camera = this->get<engine::Entity>(this->cam))
				{
					camera->ori = glm::rotate(glm::quat(), -player_agent->head_angle.x, glm::vec3(0, 0, 1));
					camera->ori = glm::rotate(camera->ori, player_agent->head_angle.y, glm::vec3(0, 1, 0));
				}

				float speed = 0.03f;
				glm::vec3 lvec[2] = {
					-glm::vec3(sin(yaw), cos(yaw), 0) * speed,
					-glm::vec3(-cos(yaw), sin(yaw), 0) * speed
				};

				// Move laterally
				player_entity->vel += lvec[0] * input_state.move_target.x;
				player_entity->vel += lvec[1] * input_state.move_target.y;

				// Jump!
				if (player_agent->on_ground)
					player_entity->vel.z += input_state.move_target.z * 0.16f;
			}
	}

	void Scene::clear()
	{
		// Remove all objects from the scene
		for (auto id : this->objects)
			this->remove(id);
	}
}
