// Local
#include <ui/win.hpp>
#include <core/scene.hpp>
#include <core/engine/entity.hpp>
#include <gfx/renderer.hpp>
#include <gfx/figure.hpp>

namespace vast
{
	extern "C" int main()
	{
		// Configure window
		ui::Win win;
		win.open();

		// Configure scene
		core::Scene scene;

		// Configure renderer
		gfx::Renderer renderer;

		// Add component variants to the scene
		scene.get_croot().add_variant(core::engine::entity_variant());
		scene.get_croot().add_variant(gfx::figure_variant());

		// Set up scene
		scene.setup();

		// Game loop
		while (win.is_open())
		{
			// Poll window events
			win.poll().except("Could not poll window");

			// Simulate the scene
			scene.tick(1.0f);

			// Render the scene to the screen
			renderer.render(scene);

			// Display the contents of the scene
			win.display().except("Could not display window contents");
		}

		win.close();

		return 0;
	}
}
