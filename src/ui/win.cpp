// Local
#include <ui/win.hpp>

// Std
#include <map>

namespace vast::ui
{
	static std::map<GLFWwindow*, Win*> callback_wins;

	static void win_resize_callback(GLFWwindow* win, int w, int h)
	{
		auto iter = callback_wins.find(win);
		if (iter != callback_wins.end())
			iter->second->resize_event(w, h);
	}

	void Win::add_callbacks_for(Win& win)
	{
		callback_wins[win._gwin] = &win;

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(win._gwin, win_resize_callback);
	}

	void Win::remove_callbacks_for(Win& win)
	{
		callback_wins.erase(win._gwin);
	}
}
