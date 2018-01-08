#ifndef VAST_HPP_UI_WIN
#define VAST_HPP_UI_WIN

// Local
#include <util/panic.hpp>

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
			return !glfwWindowShouldClose(this->_gwin);
		}

		void open()
		{
			// Suggest a 3.2 OpenGL core context
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, (int)gl::GL_TRUE);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			if ((this->_gwin = glfwCreateWindow(800, 500, "Window", nullptr, nullptr)) == nullptr)
				util::panic("Failured to create GLFW window");

			// TODO : Report OpenGL version
			//int gl_maj = glfwGetWindowAttrib(this->_gwin, GLFW_CONTEXT_VERSION_MAJOR);
			//int gl_min = glfwGetWindowAttrib(this->_gwin, GLFW_CONTEXT_VERSION_MINOR);
			//int gl_prof = glfwGetWindowAttrib(this->_gwin, GLFW_OPENGL_PROFILE);

			glfwMakeContextCurrent(this->_gwin);
		}

		void poll()
		{
			glfwPollEvents();
		}

		void close()
		{
			glfwDestroyWindow(this->_gwin);
		}

		~Win()
		{
			this->close();
		}
	};
}

#endif
