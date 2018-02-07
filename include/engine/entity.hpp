#ifndef VAST_HPP_ENGINE_ENTITY
#define VAST_HPP_ENGINE_ENTITY

// Local
#include <core/scene.hpp>

// Lib
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Std
#include <iostream>
#include <unordered_set>

namespace vast::engine
{
	// A component that encapsulates all things that have position, orientation and scale relative to another entity
	// Examples: Spacecraft, the player, humans, planets, stars, space stations, animals, items
	struct Entity
	{
		core::ObjIdent ident;

		glm::vec3 pos, vel, scale = glm::vec3(1);
		glm::quat ori, rot;
		glm::mat4 mat;

		oid_t parent_id = -1;
		std::unordered_set<oid_t> children;

		static void _tick_helper(Entity& entity, glm::mat4 parent_mat, float dt)
		{
			// Apply deltas
			entity.pos += entity.vel * dt;
			entity.ori = (entity.rot * dt) * entity.ori;

			entity.ori = glm::normalize(entity.ori);
			entity.rot = glm::normalize(entity.rot);

			// Set matrix according to new pos and ori
			entity.mat = glm::translate(parent_mat, entity.pos);
			entity.mat *= glm::mat4_cast(entity.ori); // TODO: Should this be converted to matrix first?
			entity.mat = glm::scale(entity.mat, entity.scale);
		}

		// Tick function. Allows overriding.
		// TODO: Replace this with a more general vtable system
		void (*_tick_func)(Entity& entity, glm::mat4 parent_mat, float dt) = Entity::_tick_helper;
		void set_tick_func(decltype(Entity::_tick_func) func) { this->_tick_func = func; }

		void remove_child(oid_t child_id)
		{
			this->children.erase(child_id);
		}

		void add_child(oid_t child_id)
		{
			this->children.emplace(child_id);
		}

		void set_parent(core::Scene& scene, oid_t id, oid_t nparent_id)
		{
			if (auto parent = scene.get<Entity>(this->parent_id))
				parent->remove_child(id);

			this->parent_id = nparent_id;
			if (auto nparent = scene.get<Entity>(this->parent_id))
				nparent->add_child(id);
		}

		void tick(glm::mat4 parent_mat, float dt)
		{
			// Update this entity
			this->_tick_func(*this, parent_mat, dt);

			// Tick children too
			for (oid_t child : this->children)
				if (auto entity = this->ident.scene.get<Entity>(child))
					entity->tick(this->mat, dt);
		}

		Entity(core::ObjIdent ident) : ident(ident) { std::cout << "Created Entity" << std::endl; }
		~Entity() { std::cout << "Deleted Entity" << std::endl; }
	};
}

namespace vast::core
{
	using namespace engine;

	template<> void Component<Entity>::init(Scene& scene);
	template<> void Component<Entity>::add(Scene& scene, oid_t id);
	template<> void Component<Entity>::remove(Scene& scene, oid_t id);
	template<> void Component<Entity>::tick(Scene& scene, float dt);
}

#endif
