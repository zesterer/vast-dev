#ifndef VAST_HPP_UI_WIN
#define VAST_HPP_UI_WIN

// Local
#include <util/panic.hpp>
#include <util/result.hpp>
#include <gfx/view.hpp>

// Lib
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glbinding/gl/gl.h>

namespace vast::ui
{
	struct Win
	{
		GLFWwindow* _gwin;
		gfx::View view;

		static void add_callbacks_for(Win& win);
		static void remove_callbacks_for(Win& win);

		bool is_open()
		{
			if (this->_gwin == nullptr)
				return false;
			else
				return !glfwWindowShouldClose(this->_gwin);
		}

		void open()
		{
			// Suggest a 3.2 OpenGL core context
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			if ((this->_gwin = glfwCreateWindow(this->view.width, this->view.height, "Window", nullptr, nullptr)) == nullptr)
				util::panic("Failed to create GLFW window");

			// TODO : Report OpenGL version
			//int gl_maj = glfwGetWindowAttrib(this->_gwin, GLFW_CONTEXT_VERSION_MAJOR);
			//int gl_min = glfwGetWindowAttrib(this->_gwin, GLFW_CONTEXT_VERSION_MINOR);
			//int gl_prof = glfwGetWindowAttrib(this->_gwin, GLFW_OPENGL_PROFILE);

			// Set window callbacks
			Win::add_callbacks_for(*this);

			glfwMakeContextCurrent(this->_gwin);
		}

		enum class PollError { InvalidWindow };
		USE_RESULT util::Status<PollError> poll()
		{
			if (this->_gwin == nullptr)
				return util::Status<PollError>::failure(PollError::InvalidWindow);
			else
			{
				glfwPollEvents();
				return util::Status<PollError>::success();
			}
		}

		enum class DisplayError { InvalidWindow };
		USE_RESULT util::Status<DisplayError> display()
		{
			if (this->_gwin == nullptr)
				return util::Status<DisplayError>::failure(DisplayError::InvalidWindow);
			else
			{
				glfwSwapBuffers(this->_gwin);
				return util::Status<DisplayError>::success();
			}
		}

		void close()
		{
			glfwSetWindowShouldClose(this->_gwin, GLFW_TRUE);
		}

		void resize_event(int w, int h)
		{
			this->view.width = w;
			this->view.height = h;
		}

		Win() : view(gfx::View(800, 500)) {}

		~Win()
		{
			if (this->_gwin != nullptr)
			{
				Win::remove_callbacks_for(*this);
				glfwDestroyWindow(this->_gwin);
				this->_gwin = nullptr;
			}
		}
	};
}

#endif
