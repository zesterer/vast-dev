#ifndef VAST_HPP_ENGINE_AGENT
#define VAST_HPP_ENGINE_AGENT

// Local
#include <core/scene.hpp>
#include <engine/entity.hpp>

// Lib

// Std
#include <iostream>

namespace vast::engine
{
	// A component that encapsulates all entities that hold agency (an ability to move autonomously in a player-like manner)
	// Examples: The player, aliens, wildlife, aliens, robots
	struct Agent
	{
		glm::quat head_ori;

		void tick(float dt)
		{
			(void)dt;
			// Nothing yet
		}

		Agent() { std::cout << "Created Agent" << std::endl; }
		~Agent() { std::cout << "Deleted Agent" << std::endl; }
	};
}

namespace vast::core
{
	using namespace engine;

	template<> void Component<Agent>::init(Scene& scene);
	template<> void Component<Agent>::add(Scene& scene, id_t id);
	template<> void Component<Agent>::remove(Scene& scene, id_t id);
	template<> void Component<Agent>::tick(Scene& scene, float dt);
}

#endif
