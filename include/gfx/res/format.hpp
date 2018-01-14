#ifndef VAST_HPP_GFX_RES_FORMAT
#define VAST_HPP_GFX_RES_FORMAT

// Lib
#include <glbinding/gl/gl.h>

namespace vast::gfx::res
{
	enum class FormatType { F32, I32 };

	struct Format
	{
		//std::vector<std::pair<DataType, int>> items;
		gl::GLuint gl_id;

		void bind()
		{
			gl::glBindVertexArray(this->gl_id);
		}

		Format(std::initializer_list<std::pair<FormatType, int>> items)
		{
			gl::glGenVertexArrays(1, &this->gl_id);
			this->bind();

			// Calculate stride
			size_t stride = 0;
			for (auto& attrib : items)
			{
				switch (attrib.first)
				{
				case FormatType::F32:
					stride += sizeof(gl::GLfloat) * attrib.second;
					break;
				default:
					break;
				}
			}

			size_t offset = 0;
			size_t i = 0;
			for (auto& attrib : items)
			{
				// TODO: add more data types to this
				switch(attrib.first)
				{
				case FormatType::F32:
					gl::glEnableVertexAttribArray(i);
					gl::glVertexAttribPointer(i, attrib.second, gl::GL_FLOAT, gl::GL_FALSE, stride, reinterpret_cast<void*>(offset));
					offset += sizeof(gl::GLfloat) * attrib.second;
					break;
				default:
					break;
				}

				i ++;
			}
		}
	};
}

#endif
