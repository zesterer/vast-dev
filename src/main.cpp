// Local
#include <ui/win.hpp>
#include <core/scene.hpp>
#include <core/engine/entityvar.hpp>
#include <gfx/ctx.hpp>
#include <gfx/figurevar.hpp>

namespace vast
{
	extern "C" int main()
	{
		// Configure window
		ui::Win win;
		win.open();

		// Configure scene
		core::Scene scene;
		scene.cr.add_variant(core::engine::entity_variant());
		scene.cr.add_variant(gfx::figure_variant());

		// Configure graphics context
		gfx::Ctx ctx;

		// Set up scene
		scene.setup();

		// Game loop
		while (win.is_open())
		{
			// Poll window events
			win.poll().ignore();

			// Simulate the scene
			scene.tick(1.0f);

			// TODO : Do something with this
			ctx.render();

			// Display the contents of the scene
			win.display().ignore();
		}

		win.close();

		return 0;
	}
}
