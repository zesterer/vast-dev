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
		glm::vec3 pos, vel, scale = glm::vec3(1);
		glm::quat ori, rot;
		glm::mat4 mat;

		id_t parent;
		std::unordered_set<id_t> children;

		void add_child(id_t id)
		{
			this->children.emplace(id);
		}

		void tick(core::Scene& scene, glm::mat4 space_mat, float dt)
		{
			glm::normalize(this->ori);
			glm::normalize(this->rot);

			// Apply deltas
			this->pos += this->vel * dt;
			this->ori = (this->rot * dt) * this->ori;

			// Set matrix according to new pos and ori
			this->mat = glm::translate(space_mat, this->pos);
			this->mat *= glm::mat4_cast(this->ori); // TODO: Should this be converted to matrix first?
			this->mat = glm::scale(this->mat, this->scale);

			// Tick children too
			for (id_t child : this->children)
				if (auto entity = scene.get<Entity>(child))
					entity->tick(scene, this->mat, dt);
		}

		Entity() { std::cout << "Created Entity" << std::endl; }
		~Entity() { std::cout << "Deleted Entity" << std::endl; }
	};
}

namespace vast::core
{
	using namespace engine;

	template<> void Component<Entity>::init(Scene& scene);
	template<> void Component<Entity>::add(Scene& scene, id_t id);
	template<> void Component<Entity>::remove(Scene& scene, id_t id);
	template<> void Component<Entity>::tick(Scene& scene, float dt);
}

#endif
