#ifndef VAST_HPP_GFX_PIPELINE
#define VAST_HPP_GFX_PIPELINE

// Local
#include <gfx/res/shader.hpp>
#include <gfx/res/texture.hpp>
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
		enum class Error { NO_SUCH_UNIFORM, INVALID_UNIFORM };

		res::Shader shader;
		Target target;

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
			if (uniform.gl_id == -1)
				return util::Status<Error>::failure(Error::INVALID_UNIFORM);
			else
			{
				gl::glUniform1f(uniform.gl_id, f);
				return util::Status<Error>::success();
			}
		}

		util::Status<Error> set_uniform(Uniform uniform, glm::mat4 const& m)
		{
			if (uniform.gl_id == -1)
				return util::Status<Error>::failure(Error::INVALID_UNIFORM);
			else
			{
				gl::glUniformMatrix4fv(uniform.gl_id, 1, gl::GL_FALSE, &m[0][0]);
				return util::Status<Error>::success();
			}
		}

		util::Status<Error> set_uniform(Uniform uniform, int i)
		{
			if (uniform.gl_id == -1)
				return util::Status<Error>::failure(Error::INVALID_UNIFORM);
			else
			{
				gl::glUniform1i(uniform.gl_id, i);
				return util::Status<Error>::success();
			}
		}

		void bind() const
		{
			this->shader.use();
			this->target.bind();
		}

		void bind_texture(Uniform uniform, res::Texture const& tex, int index = 0)
		{
			this->set_uniform(uniform, index);
			gl::glActiveTexture(gl::GL_TEXTURE0 + index);
			tex.bind();
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
