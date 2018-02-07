#ifndef VAST_HPP_ENGINE_VESSEL
#define VAST_HPP_ENGINE_VESSEL

// Local
#include <core/scene.hpp>
#include <engine/entity.hpp>
#include <engine/phys/aabb.hpp>

// Lib
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

// Std
#include <iostream>
#include <utility>

namespace vast::engine
{
	// A component that encapsulates all voxel-based dynamic bodies.
	// Examples: Spacecraft, space station, asteroids, submarines, land vehicles, rocks
	struct Vessel
	{
		core::ObjIdent ident;

		glm::vec3 center;

		bool check_collision(phys::AABB3D box);
		std::pair<phys::AABB3D, glm::vec3> resolve_collision(phys::AABB3D box, glm::vec3 delta_pos);

		void tick(float dt)
		{
			(void)dt;

			if (auto entity = this->ident.scene.get<Entity>(this->ident.id))
			{
				entity->vel = glm::vec3(0.1f, 0, 0) * entity->ori;
				entity->rot = glm::rotate(
					entity->rot,
					(rand() % 100 - 50) * 0.000001f,
					glm::normalize(glm::vec3(0, 0, 1))
				);
			}
		}

		Vessel(core::ObjIdent ident) : ident(ident), center(glm::vec3(0)) { std::cout << "Created Vessel" << std::endl; }
		~Vessel() { std::cout << "Deleted Vessel" << std::endl; }
	};
}

namespace vast::core
{
	using namespace engine;

	template<> void Component<Vessel>::init(Scene& scene);
	template<> void Component<Vessel>::add(Scene& scene, oid_t id);
	template<> void Component<Vessel>::remove(Scene& scene, oid_t id);
	template<> void Component<Vessel>::tick(Scene& scene, float dt);
}

#endif
