#ifndef VAST_HPP_ENGINE_PHYS_AABB
#define VAST_HPP_ENGINE_PHYS_AABB

// Lib
#include <glm/glm.hpp>

namespace vast::engine::phys
{
	template <typename T>
	struct AABB
	{
		T pos, size;
		AABB(T pos, T size) : pos(pos), size(size) {}
	};

	using AABB3D = AABB<glm::vec3>;
}

#endif
