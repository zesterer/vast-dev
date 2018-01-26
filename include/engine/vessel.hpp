#ifndef VAST_HPP_ENGINE_VESSEL
#define VAST_HPP_ENGINE_VESSEL

// Local
#include <core/scene.hpp>

// Lib
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

// Std
#include <iostream>

namespace vast::engine
{
	struct Vessel
	{
		void tick(float dt)
		{
			// Nothing yet
		}

		Vessel() { std::cout << "Created Vessel" << std::endl; }
		~Vessel() { std::cout << "Deleted Vessel" << std::endl; }
	};
}

namespace vast::core
{
	using namespace engine;

	template<> void Component<Vessel>::init(Scene& scene);
	template<> void Component<Vessel>::add(Scene& scene, id_t id);
	template<> void Component<Vessel>::remove(Scene& scene, id_t id);
	template<> void Component<Vessel>::tick(Scene& scene, float dt);
}

#endif
