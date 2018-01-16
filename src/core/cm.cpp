// Local
#include <core/cm.hpp>

namespace vast::core
{
	ComponentVariant::ComponentVariant(
		int variant_id,
		init_func_t init,
		remove_func_t remove,
		tick_func_t tick
	) :
		variant_id(variant_id),
		init(init),
		remove(remove),
		tick(tick)
	{}

	int cm_component_counter = 0;
	int cm_register_component()
	{
		return ++cm_component_counter;
	}
}
