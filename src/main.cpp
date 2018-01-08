// Local
#include <core/scene.hpp>
#include <ui/win.hpp>

namespace vast
{
	extern "C" int main()
	{
		core::Scene scene;
		ui::Win win;

		// Open the window, run the game loop
		win.open();
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
