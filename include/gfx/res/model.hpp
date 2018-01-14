#ifndef VAST_HPP_GFX_RES_MODEL
#define VAST_HPP_GFX_RES_MODEL

// Local
#include <gfx/res/mesh.hpp>

// Lib
#include <glbinding/gl/gl.h>

namespace vast::gfx::res
{
	struct Model
	{
		gl::GLuint gl_id;

		void bind()
		{
			gl::glBindBuffer(gl::GL_ARRAY_BUFFER, this->gl_id);
		}

		Model(Mesh& mesh)
		{
			gl::glGenBuffers(1, &this->gl_id);
			this->bind();

			gl::glBufferData(gl::GL_ARRAY_BUFFER, sizeof(Vert) * 3 * mesh.size(), mesh.get_data(), gl::GL_STATIC_DRAW);
		}
	};
}

#endif
