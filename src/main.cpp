// Local
#include <core/scene.hpp>
#include <core/entity.hpp>
#include <ui/win.hpp>

namespace vast
{
	extern "C" int main()
	{
		// Configure scene
		core::Scene scene;
		scene.add_component_type(core::entity_component_type());

		// Set up scene
		scene.setup();

		// Configure window
		ui::Win win;
		win.open();

		// Game loop
		while (win.is_open())
		{
			win.poll().ignore();

			scene.tick(1.0f);

			win.display().ignore();
		}

		win.close();

		return 0;
	}
}
