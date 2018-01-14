#ifndef VAST_HPP_GFX_PIPELINE
#define VAST_HPP_GFX_PIPELINE

// Local
#include <gfx/res/shader.hpp>
#include <gfx/format.hpp>

// Std
#include <string>
#include <initializer_list>

namespace vast::gfx
{
	struct Target
	{
		void bind() const
		{
			// Nothing yet
		}
	};

	struct Pipeline
	{
		res::Shader shader;
		Target target;

		void bind() const
		{
			this->shader.use();
			this->target.bind();
		}

		Pipeline(
			res::Shader shader,
			Target target
		) :
			shader(shader),
			target(target)
		{}
	};
}

#endif
