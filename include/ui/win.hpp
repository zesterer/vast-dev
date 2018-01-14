#ifndef VAST_HPP_UI_WIN
#define VAST_HPP_UI_WIN

// Local
#include <util/panic.hpp>
#include <util/result.hpp>

// Lib
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glbinding/gl/gl.h>

namespace vast::ui
{
	struct Win
	{
		GLFWwindow* _gwin;

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

			if ((this->_gwin = glfwCreateWindow(800, 500, "Window", nullptr, nullptr)) == nullptr)
				util::panic("Failed to create GLFW window");

			// TODO : Report OpenGL version
			//int gl_maj = glfwGetWindowAttrib(this->_gwin, GLFW_CONTEXT_VERSION_MAJOR);
			//int gl_min = glfwGetWindowAttrib(this->_gwin, GLFW_CONTEXT_VERSION_MINOR);
			//int gl_prof = glfwGetWindowAttrib(this->_gwin, GLFW_OPENGL_PROFILE);

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
			if (this->_gwin != nullptr)
			{
				glfwDestroyWindow(this->_gwin);
				this->_gwin = nullptr;
			}
		}

		~Win()
		{
			this->close();
		}
	};
}

#endif
