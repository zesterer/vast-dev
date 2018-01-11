// Local
#include <core/cm.hpp>

namespace vast::core
{
	ComponentVariant::ComponentVariant(
		int variant_id,
		std::function<void (ComponentRoot&, id_t)> create,
		std::function<void (ComponentRoot&, id_t)> remove,
		std::function<void (ComponentRoot&, float)> tick
	) :
		variant_id(variant_id),
		create(create),
		remove(remove),
		tick(tick)
	{}

	int cm_component_counter = 0;
	int cm_register_component()
	{
		return ++cm_component_counter;
	}
}
