#ifndef VAST_HPP_CORE_CM
#define VAST_HPP_CORE_CM

// Std
#include <functional>

namespace vast::core
{
	typedef size_t cm_id;
	typedef size_t id_t;

	struct ComponentRoot
	{
		cm_id id;
	};

	struct Component
	{
		std::function<void (ComponentRoot&, int, id_t)> create;
		std::function<void (ComponentRoot&, float)> tick;

		Component(
			std::function<void (ComponentRoot&, int, id_t)> create,
			std::function<void (ComponentRoot&, float)> tick
		) :
			create(create),
			tick(tick)
		{}
	};
}

#endif