#ifndef VAST_HPP_GFX_PIPELINE
#define VAST_HPP_GFX_PIPELINE

// Local
#include <gfx/res/format.hpp>
#include <gfx/res/shader.hpp>

// Std
#include <string>
#include <initializer_list>

namespace vast::gfx
{
	struct Target
	{
		// Nothing yet
	};

	struct Pipeline
	{
		res::Format format;
		res::Shader shader;
		Target target;

		Pipeline(
			res::Format format,
			res::Shader shader,
			Target target
		) :
			format(format),
			shader(shader),
			target(target)
		{}
	};
}

#endif
