#ifndef VAST_HPP_GFX_RENDERER
#define VAST_HPP_GFX_RENDERER

// Local
#include <core/scene.hpp>
#include <engine/entity.hpp>
#include <gfx/figure.hpp>
#include <gfx/camera.hpp>
#include <gfx/view.hpp>

// Lib
#include <glbinding/Binding.h>
#include <glbinding/gl/gl.h>
#include <glm/glm.hpp>

namespace vast::gfx
{
	struct Renderer
	{
		// Clear the screen of existing features ready for a new frame
		void clear(glm::vec3 color = glm::vec3(0, 0, 0))
		{
			gl::glClearColor(color.r, color.g, color.b, 1.0);
			gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);
		}

		// Render a scene to the screen
		void render(core::Scene& scene, View const& view)
		{
			// Use the current context for rendering (TODO: Switch context based on window?)
			glbinding::Binding::useCurrentContext();

			// Size the viewport appropriately
			gl::glViewport(0, 0, view.width, view.height);

			// Create a camera for the current frame, and update according to camera entity
			Camera cam(90.0f);
			cam.set_view(view);
			if (auto entity = core::Component<engine::Entity>::get(scene, scene.cam))
				cam.update_from(*entity);

			// Clear the screen ready for the next frame of rendering
			this->clear(glm::vec3(0, 0, 1.1));

			// Render each figure object in the scene
			render_figures(scene, cam);
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
					if (false)
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
				}
			);

			gl::glEnable(gl::GL_CULL_FACE);
			gl::glEnable(gl::GL_DEPTH_TEST);
			gl::glDepthFunc(gl::GL_LESS);
		}
	};
}

#endif
