// Local
#include <ui/win.hpp>
#include <core/scene.hpp>
#include <engine/entity.hpp>
#include <engine/volume.hpp>
#include <engine/vessel.hpp>
#include <gfx/renderer.hpp>
#include <gfx/figure.hpp>
#include <gfx/body.hpp>

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
		scene.calls.push_back(core::Component<engine::Entity>::get_calls());
		scene.calls.push_back(core::Component<engine::Volume>::get_calls());
		scene.calls.push_back(core::Component<engine::Vessel>::get_calls());
		scene.calls.push_back(core::Component<gfx::Figure>::get_calls());
		scene.calls.push_back(core::Component<gfx::Body>::get_calls());

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
			renderer.render(scene, win.view);

			// Display the contents of the scene
			win.display().except("Could not display window contents");
		}

		win.close();

		return 0;
	}
}
