#ifndef VAST_HPP_GFX_CTX
#define VAST_HPP_GFX_CTX

// Lib
#include <glbinding/Binding.h>
#include <glbinding/gl/gl.h>
#include <glm/glm.hpp>

namespace vast::gfx
{
	struct Ctx
	{
		// Clear the screen of existing features ready for a new frame
		void clear(glm::vec3 color = glm::vec3(1, 1, 1))
		{
			gl::glEnable(gl::GL_CULL_FACE);
			gl::glEnable(gl::GL_DEPTH_TEST);
			gl::glDepthFunc(gl::GL_LESS);

			gl::glClearColor(color.r, color.g, color.b, 1.0);
			gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);
		}

		// Render everything to the screen
		void render()
		{
			// Use the current context for rendering (TODO: Switch context based on window?)
			glbinding::Binding::useCurrentContext();

			// Clear the screen ready for the next frame of rendering
			this->clear();
		}

		Ctx()
		{
			// Lazy glbinding initialization to avoid doing it twice
			glbinding::Binding::initialize(false);
		}
	};
}

#endif
