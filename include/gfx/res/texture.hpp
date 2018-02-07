#ifndef VAST_HPP_GFX_RES_TEXTURE
#define VAST_HPP_GFX_RES_TEXTURE

// Local
#include <gfx/res/image.hpp>
#include <util/panic.hpp>

// Lib
#include <glm/glm.hpp>
#include <glbinding/gl/gl.h>

namespace vast::gfx::res
{
	struct Texture
	{
		bool _valid;
		gl::GLuint gl_id;
		gl::GLenum gl_format = gl::GL_RGB;
		glm::ivec2 size;

		bool is_valid()
		{
			return this->_valid;
		}

		void bind() const
		{
			if (this->_valid)
				gl::glBindTexture(gl::GL_TEXTURE_2D, this->gl_id);
			else
				util::panic("Attempted to bind invalid texture");
		}

		void set_image(Image const& image)
		{
			this->bind();

			this->size = image.size;

			// Load pixel data into texture
			gl::glTexImage2D(gl::GL_TEXTURE_2D, 0, (gl::GLint)gl::GL_RGB, this->size.x, this->size.y, 0, this->gl_format, gl::GL_UNSIGNED_BYTE, &image.data[0]);

			// Enable mip-mapping
			gl::glTexParameteri(gl::GL_TEXTURE_2D, gl::GL_TEXTURE_MAG_FILTER, (gl::GLuint)gl::GL_LINEAR);
			gl::glTexParameteri(gl::GL_TEXTURE_2D, gl::GL_TEXTURE_MIN_FILTER, (gl::GLuint)gl::GL_LINEAR);
		}

		void unload()
		{
			if (this->_valid)
				gl::glDeleteTextures(1, &this->gl_id);
		}

		Texture() : _valid(false), size(glm::ivec2(0)) {}
		Texture(Image const& image) : _valid(true), size(image.size)
		{
			// Generate and bind VAO
			gl::glGenTextures(1, &this->gl_id);

			this->set_image(image);
		}

		Texture(Texture&& other) :
			_valid(other._valid),
			gl_id(other.gl_id),
			gl_format(other.gl_format),
			size(other.size)
		{
			other._valid = false;
		}

		Texture& operator=(Texture&& other)
		{
			this->unload();

			this->_valid = other._valid;
			this->gl_id = other.gl_id;
			this->gl_format = other.gl_format;
			this->size = other.size;

			other._valid = false;

			return *this;
		}

		~Texture()
		{
			this->unload();
		}
	};
}

#endif
