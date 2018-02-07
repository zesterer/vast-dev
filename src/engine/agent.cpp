// Local
#include <engine/agent.hpp>
#include <engine/entity.hpp>
#include <engine/vessel.hpp>

namespace vast::core
{
	using namespace engine;

	template<> ComponentBox<Agent> Component<Agent>::box = ComponentBox<Agent>();

	template<> void Component<Agent>::init(Scene& scene)
	{
		(void)scene;

		std::cout << "Agent component initiated!" << std::endl;
	}

	void agent_entity_tick(Entity& entity, glm::mat4 parent_mat, float dt)
	{
		auto agent = entity.ident.scene.get<Agent>(entity.ident.id);
		auto vessel = entity.ident.scene.get<Vessel>(entity.parent_id);

		// Apply drag
		const glm::vec3 drag_factor = glm::vec3(0.2f, 0.2f, 0.0f);
		entity.vel -= glm::abs(entity.vel) * (entity.vel * drag_factor) + entity.vel * drag_factor;

		// Apply gravity
		entity.vel.z -= 0.01f * dt;

		// Apply position delta
		if (agent && vessel) // Vessel collision
		{
			float previous_vel_z = entity.vel.z;

			glm::vec3 offset = glm::vec3(agent->size.x, agent->size.y, 0) / 2.0f;
			auto box_delta = vessel->resolve_collision(phys::AABB3D(entity.pos - offset, agent->size), entity.vel * dt);
			entity.pos = box_delta.first.pos + offset;
			entity.vel = box_delta.second;

			// Are we on the ground? Set a flag
			agent->on_ground = entity.vel.z == 0 && previous_vel_z < entity.vel.z;
		}
		else
		{
			entity.pos += entity.vel * dt;

			// We're not on the ground
			if (agent)
				agent->on_ground = false;
		}

		// Apply orientation delta
		entity.ori = (entity.rot * dt) * entity.ori;

		entity.ori = glm::normalize(entity.ori);
		entity.rot = glm::normalize(entity.rot);

		// Set matrix according to new pos and ori
		entity.mat = glm::translate(parent_mat, entity.pos);
		entity.mat *= glm::mat4_cast(entity.ori); // TODO: Should this be converted to matrix first?
		entity.mat = glm::scale(entity.mat, entity.scale);
	}

	template<> void Component<Agent>::add(Scene& scene, oid_t id)
	{
		// Add entity first
		Component<Entity>::add(scene, id);
		if (auto entity = scene.get<Entity>(id))
			entity->set_tick_func(agent_entity_tick);

		self::box.emplace(scene, id, ObjIdent(id, scene));
	}

	template<> void Component<Agent>::tick(Scene& scene, float dt)
	{
		for (auto agent : self::box.items(scene))
			agent.second->tick(dt);
	}

	template<> void Component<Agent>::remove(Scene& scene, oid_t id)
	{
		self::box.remove(scene, id);
	}
}
