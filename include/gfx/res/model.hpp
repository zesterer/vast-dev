#ifndef VAST_HPP_GFX_RES_MODEL
#define VAST_HPP_GFX_RES_MODEL

// Local
#include <gfx/res/mesh.hpp>
#include <gfx/pipeline.hpp>
#include <gfx/format.hpp>
#include <util/panic.hpp>

// Lib
#include <glbinding/gl/gl.h>

namespace vast::gfx::res
{
	struct Model
	{
		bool _valid;
		gl::GLuint gl_id;
		gl::GLenum gl_primitive;
		size_t vertex_count;

		void bind() const
		{
			if (this->_valid)
				gl::glBindBuffer(gl::GL_ARRAY_BUFFER, this->gl_id);
			else
				util::panic("Attempted to bind invalid model");
		}

		Model() : _valid(false), vertex_count(0) {}
		Model(
			Pipeline const& pipeline,
			Mesh const& mesh,
			gl::GLenum gl_primitive,
			std::initializer_list<std::tuple<std::string, FormatType, int>> attrs
		) :
			_valid(true),
			gl_primitive(gl_primitive),
			vertex_count(mesh.get_vertex_count())
		{
			// Generate and bind VAO
			gl::glGenVertexArrays(1, &this->gl_id);
			gl::glBindVertexArray(this->gl_id);

			// Generate and bind VBO
			gl::GLuint vbo_gl_id;
			gl::glGenBuffers(1, &vbo_gl_id);
			gl::glBindBuffer(gl::GL_ARRAY_BUFFER, vbo_gl_id);

			// Load data into VBO
			gl::glBufferData(gl::GL_ARRAY_BUFFER, sizeof(Vert) * mesh.get_vertex_count(), mesh.get_data(), gl::GL_STATIC_DRAW);

			// Bind format
			bind_format_attributes(pipeline.shader, this->gl_id, attrs);
		}
	};
}

#endif
