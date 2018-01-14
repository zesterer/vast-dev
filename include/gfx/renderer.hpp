#ifndef VAST_HPP_GFX_RENDERER
#define VAST_HPP_GFX_RENDERER

// Local
#include <core/scene.hpp>
#include <gfx/figure.hpp>

// Lib
#include <glbinding/Binding.h>
#include <glbinding/gl/gl.h>
#include <glm/glm.hpp>

namespace vast::gfx
{
	struct Renderer
	{
		// Clear the screen of existing features ready for a new frame
		void clear(glm::vec3 color = glm::vec3(1, 1, 1))
		{
			gl::glClearColor(color.r, color.g, color.b, 1.0);
			gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);
		}

		// Render a figure
		void render_figure(Figure const& figure)
		{
			figure.pipeline.bind();
			figure.model.bind();

			gl::glDrawArrays(figure.model.gl_primitive, 0, figure.model.vertex_count);
		}

		// Render a scene to the screen
		void render(core::Scene const& scene)
		{
			// Use the current context for rendering (TODO: Switch context based on window?)
			glbinding::Binding::useCurrentContext();

			// Clear the screen ready for the next frame of rendering
			this->clear();

			// Render each figure object in the scene
			for (id_t obj : scene.get_objects())
			{
				if (auto figure = figure_get(scene.get_croot(), obj))
					render_figure(**figure);
			}
		}

		Renderer()
		{
			// Lazy glbinding initialization to avoid doing it twice
			glbinding::Binding::initialize(false);
			glbinding::Binding::useCurrentContext();

			// Set logging callback
			glbinding::setCallbackMaskExcept(glbinding::CallbackMask::After, { "glGetError" });
			glbinding::setCallbackMask(glbinding::CallbackMask::After | glbinding::CallbackMask::ParametersAndReturnValue);
			glbinding::setAfterCallback(
				[] (glbinding::FunctionCall const& c)
				{
					if (std::string(c.function->name()) == "glGetError")
						return;

					std::cout << c.function->name() << "(";
					for (size_t i = 0; i < c.parameters.size(); i ++)
					{
						std::cout << c.parameters[i]->asString();
						if (i < c.parameters.size() - 1)
							std::cout << ", ";
					}
					std::cout << ")";

					if (c.returnValue)
						std::cout << " -> " << c.returnValue->asString();
					std::cout << std::endl;

					const auto err = gl::glGetError();
					if (err != gl::GL_NO_ERROR)
						std::cout << "GL error: " << err << std::endl;
				}
			);

			//gl::glEnable(gl::GL_CULL_FACE);
			//gl::glEnable(gl::GL_DEPTH_TEST);
			//gl::glDepthFunc(gl::GL_LESS);
		}
	};
}

#endif
