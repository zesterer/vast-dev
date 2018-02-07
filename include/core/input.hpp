#ifndef VAST_HPP_CORE_INPUT
#define VAST_HPP_CORE_INPUT

// Lib
#include <glm/glm.hpp>

namespace vast::core
{
	struct InputState
	{
		glm::vec2 look_offset;
		glm::vec3 move_target;

		InputState() :
			look_offset(glm::vec2(0)),
			move_target(glm::vec3(0))
		{}
	};
}

#endif