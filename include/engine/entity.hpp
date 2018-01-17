#ifndef VAST_HPP_ENGINE_ENTITY
#define VAST_HPP_ENGINE_ENTITY

// Local
#include <core/scene.hpp>

// Lib
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

// Std
#include <iostream>

namespace vast::engine
{
	struct Entity
	{
		glm::vec3 pos, vel, scale = glm::vec3(1);
		glm::quat ori, rot;

		void tick(float dt)
		{
			glm::normalize(this->ori);
			glm::normalize(this->rot);

			this->pos += this->vel * dt; // Apply velocity
			this->ori = (this->rot * dt) * this->ori; // Apply rotation
		}

		Entity() { std::cout << "Created Entity" << std::endl; }
		~Entity() { std::cout << "Deleted Entity" << std::endl; }
	};
}

namespace vast::core
{
	using namespace engine;

	template<> void Component<Entity>::init(Scene& scene);
	template<> id_t Component<Entity>::create(Scene& scene);
	template<> void Component<Entity>::remove(Scene& scene, id_t id);
	template<> void Component<Entity>::tick(Scene& scene, float dt);
}

#endif
