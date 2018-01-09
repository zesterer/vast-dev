// Local
#include <core/cm.hpp>

namespace vast::core
{
	int cm_component_counter = 0;
	int cm_register_component()
	{
		return ++cm_component_counter;
	}
}
