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
		core::ObjIdent ident;

		glm::vec2 head_angle;
		glm::vec3 size;
		bool on_ground = false;

		void tick(float dt)
		{
			(void)dt;
			// Nothing yet
		}

		Agent(core::ObjIdent ident) : ident(ident), head_angle(glm::vec2(0)), size(glm::vec3(1)) { std::cout << "Created Agent" << std::endl; }
		~Agent() { std::cout << "Deleted Agent" << std::endl; }
	};
}

namespace vast::core
{
	using namespace engine;

	template<> void Component<Agent>::init(Scene& scene);
	template<> void Component<Agent>::add(Scene& scene, oid_t id);
	template<> void Component<Agent>::remove(Scene& scene, oid_t id);
	template<> void Component<Agent>::tick(Scene& scene, float dt);
}

#endif
