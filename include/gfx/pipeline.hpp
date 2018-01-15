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
	struct Uniform
	{
		gl::GLint gl_id;

		Uniform(gl::GLint gl_id) : gl_id(gl_id) {}
	};

	struct Target
	{
		void bind() const
		{
			// Nothing yet
		}
	};

	struct Pipeline
	{
		enum class Error { NO_SUCH_UNIFORM };

		res::Shader shader;
		Target target;

		void bind() const
		{
			this->shader.use();
			this->target.bind();
		}

		util::Result<Uniform, Error> get_uniform(std::string const& str) const
		{
			gl::GLint gl_id = gl::glGetUniformLocation(this->shader.gl_id, str.c_str());
			if (gl_id == -1)
				return util::Result<Uniform, Error>::failure(Error::NO_SUCH_UNIFORM);
			else
				return util::Result<Uniform, Error>::success(Uniform(gl_id));
		}

		util::Status<Error> set_uniform(Uniform uniform, float f)
		{
			gl::glUniform1f(uniform.gl_id, f);
			return util::Status<Error>::success();
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
