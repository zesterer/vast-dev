// Local
#include <gfx/format.hpp>
#include <gfx/res/shader.hpp>

namespace vast::gfx
{
	void bind_format_attributes(gl::GLuint vao_gl_id, std::initializer_list<std::tuple<std::string, FormatType, int>> attrs)
	{
		// Calculate stride
		size_t stride = 0;
		for (auto& attr : attrs)
		{
			switch (std::get<1>(attr))
			{
			case FormatType::F32:
				stride += sizeof(gl::GLfloat) * std::get<2>(attr);
				break;
			default:
				break;
			}
		}

		gl::glBindVertexArray(vao_gl_id);

		size_t offset = 0;
		size_t i = 0;
		for (auto& attr : attrs)
		{
			gl::glEnableVertexAttribArray(i);

			// TODO: add more data types to this
			switch(std::get<1>(attr))
			{
			case FormatType::F32:
				gl::glVertexAttribPointer(i, std::get<2>(attr), gl::GL_FLOAT, gl::GL_FALSE, stride, reinterpret_cast<void*>(offset));
				offset += sizeof(gl::GLfloat) * std::get<2>(attr);
				break;
			default:
				break;
			}

			i ++;
		}
	}
}
