#ifndef VAST_HPP_GFX_FORMAT
#define VAST_HPP_GFX_FORMAT

// Local
#include <gfx/res/shader.hpp>

// Lib
#include <glbinding/gl/gl.h>

// Std
#include <initializer_list>

namespace vast::gfx
{
	enum class FormatType { F32, I32 };

	void bind_format_attributes(res::Shader const& shader, gl::GLuint vao_gl_id, std::initializer_list<std::tuple<std::string, FormatType, int>> attrs);
}

#endif
