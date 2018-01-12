#ifndef VAST_HPP_GFX_PIPELINE
#define VAST_HPP_GFX_PIPELINE

// Local
#include <gfx/shader.hpp>

// Std
#include <string>

namespace vast::gfx
{
	enum class DataType { F32, I32 };

	struct InputFormat
	{
		std::vector<std::pair<DataType, int>> items;
	};

	struct Target
	{
		// Nothing yet
	};

	struct Pipeline
	{
		InputFormat input_format;
		Shader shader;
		Target target;

		Pipeline(
			InputFormat input_format,
			Shader shader,
			Target target
		) :
			input_format(input_format),
			shader(shader),
			target(target)
		{}
	};
}

#endif
