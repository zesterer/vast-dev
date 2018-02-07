#ifndef VAST_HPP_UI_WIN
#define VAST_HPP_UI_WIN

// Local
#include <core/input.hpp>
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

		core::InputState input_state;
		bool cursor_trapped;

		static void add_callbacks_for(Win& win);
		static void remove_callbacks_for(Win& win);

		bool is_open()
		{
			if (this->_gwin == nullptr)
				return false;
			else
				return !glfwWindowShouldClose(this->_gwin);
		}

		void set_cursor_trapped(bool trapped)
		{
			this->cursor_trapped = trapped;

			if (this->cursor_trapped)
				glfwSetInputMode(this->_gwin, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			else
				glfwSetInputMode(this->_gwin, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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

			// Switch active GL context to the context in this window
			glfwMakeContextCurrent(this->_gwin);

			// Trap the cursor
			this->set_cursor_trapped(this->cursor_trapped);
		}

		enum class PollError { InvalidWindow };
		USE_RESULT util::Status<PollError> poll()
		{
			if (this->_gwin == nullptr)
				return util::Status<PollError>::failure(PollError::InvalidWindow);
			else
			{
				// Poll window events
				glfwPollEvents();

				if (this->cursor_trapped)
				{
					// Record cursor position, reset cursor, and remember offset
					double
						tgt_x = this->view.width / 2,
						tgt_y = this->view.height / 2
					;
					double cursor_x, cursor_y;
					glfwGetCursorPos(this->_gwin, &cursor_x, &cursor_y);
					glfwSetCursorPos(this->_gwin, tgt_x, tgt_y);

					this->input_state.look_offset.x = static_cast<float>(cursor_x - tgt_x);
					this->input_state.look_offset.y = static_cast<float>(cursor_y - tgt_y);
				}

				// Receive keyboard press input
				this->input_state.move_target = glm::vec3(0);

				this->input_state.move_target.x =
					((glfwGetKey(this->_gwin, GLFW_KEY_D) == GLFW_PRESS) ? 1 : 0) -
					((glfwGetKey(this->_gwin, GLFW_KEY_A) == GLFW_PRESS) ? 1 : 0)
				;
				this->input_state.move_target.y =
					((glfwGetKey(this->_gwin, GLFW_KEY_W) == GLFW_PRESS) ? 1 : 0) -
					((glfwGetKey(this->_gwin, GLFW_KEY_S) == GLFW_PRESS) ? 1 : 0)
				;
				this->input_state.move_target.z =
					((glfwGetKey(this->_gwin, GLFW_KEY_SPACE) == GLFW_PRESS) ? 1 : 0) -
					((glfwGetKey(this->_gwin, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) ? 1 : 0)
				;


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

		Win() : view(gfx::View(800, 500)), cursor_trapped(true) {}

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
