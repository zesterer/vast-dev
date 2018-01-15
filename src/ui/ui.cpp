// Local
#include <ui/win.hpp>
#include <util/panic.hpp>

// Lib
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace vast::ui
{
	// Initiate the GLFW library at program start
	__attribute__ ((constructor)) void ui_init()
	{
		if (!glfwInit())
			util::panic("Failed to init GLFW");
	}

	// Terminate the GLFW library at program start
	__attribute__ ((destructor)) void ui_stop()
	{
		glfwTerminate();
	}
}
