// Local
#include <ui/win.hpp>
#include <util/result.hpp>

namespace vast
{
	ui::Win win;

	extern "C" int main()
	{
		win.open();
		while (win.is_open())
		{
			win.poll();
		}

		win.close();

		return 0;
	}
}
