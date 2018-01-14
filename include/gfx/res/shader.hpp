#ifndef VAST_HPP_GFX_RES_SHADER
#define VAST_HPP_GFX_RES_SHADER

// Local
#include <util/result.hpp>

// Lib
#include <glbinding/gl/gl.h>

// Std
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>

namespace vast::gfx::res
{
	struct Shader
	{
	public:
		gl::GLuint gl_id;

		void use() const
		{
			gl::glUseProgram(this->gl_id);
		}

		Shader(gl::GLuint gl_id) : gl_id(gl_id) {}

		enum class Error { CANNOT_OPEN_FILE, COMPILE_ERROR, LINK_ERROR };

		static util::Result<Shader, Error> from(gl::GLuint vert, gl::GLuint frag)
		{
			gl::GLuint gl_id = gl::glCreateProgram();
			gl::glAttachShader(gl_id, vert);
			gl::glAttachShader(gl_id, frag);
			gl::glLinkProgram(gl_id);

			gl::glDetachShader(gl_id, vert);
			gl::glDetachShader(gl_id, frag);

			gl::GLint result = 0, log_len = 0;
			gl::glGetProgramiv(gl_id, gl::GL_LINK_STATUS, &result);
			if (result == (gl::GLint)gl::GL_FALSE)
			{
				gl::glGetProgramiv(gl_id, gl::GL_INFO_LOG_LENGTH, &log_len);
				std::vector<char> log_buff(log_len + 1);
				gl::glGetProgramInfoLog(gl_id, log_len, nullptr, &log_buff[0]);
				std::cout << "Shader link error: " << std::string(&log_buff[0]) << std::endl;

				gl::glDeleteProgram(gl_id);

				return util::Result<Shader, Error>::failure(Error::LINK_ERROR);
			}
				return util::Result<Shader, Error>::success(Shader(gl_id));
		}

		static util::Result<Shader, Error> from_strings(std::string const& vert_str, std::string const& frag_str)
		{
			auto create_shader = [](gl::GLenum type, std::string const& str) -> util::Result<gl::GLuint, std::string>
			{
				gl::GLuint gl_id = gl::glCreateShader(type);
				const char* src_buff = str.c_str();
				gl::glShaderSource(gl_id, 1, &src_buff, nullptr);
				gl::glCompileShader(gl_id);

				gl::GLint result = 0, log_len = 0;
				gl::glGetShaderiv(gl_id, gl::GL_COMPILE_STATUS, &result);
				if (result == (gl::GLint)gl::GL_FALSE)
				{
					gl::glGetShaderiv(gl_id, gl::GL_INFO_LOG_LENGTH, &log_len);
					std::vector<char> log_buff(log_len + 1);
					gl::glGetShaderInfoLog(gl_id, log_len, nullptr, &log_buff[0]);
					std::cout << "Shader compilation error: " << std::string(&log_buff[0]) << std::endl;
					return util::Result<gl::GLuint, std::string>::failure(std::string(&log_buff[0]));
				}
				else
					return util::Result<gl::GLuint, std::string>::success(gl_id);
			};

			// Create the shaders
			auto vert = create_shader(gl::GL_VERTEX_SHADER, vert_str);
			auto frag = create_shader(gl::GL_FRAGMENT_SHADER, frag_str);
			if (vert && frag)
			{
				auto shader = Shader::from(*vert, *frag);

				gl::glDeleteShader(*vert);
				gl::glDeleteShader(*frag);

				if (shader)
					return util::Result<Shader, Error>::success(*shader);
				else
					return util::Result<Shader, Error>::failure(shader.get_error());
			}
			else
				return util::Result<Shader, Error>::failure(Error::COMPILE_ERROR);
		}

		static util::Result<Shader, Error> from_files(std::string const& vert_filename, std::string const& frag_filename)
		{
			auto read_file = [](std::string const& filename) -> util::Result<std::string, Error>
			{
				std::ifstream fstream(filename, std::ios::in);
				if (!fstream.is_open())
					return util::Result<std::string, Error>::failure(Error::CANNOT_OPEN_FILE);
				else
				{
					std::stringstream sstream;
					sstream << fstream.rdbuf();
					return util::Result<std::string, Error>::success(sstream.str());
				}
			};

			std::string vert_text, frag_text;

			// Read files
			auto vert = read_file(vert_filename);
			auto frag = read_file(frag_filename);
			if (vert && frag)
			{
				if (auto shader = Shader::from_strings(*vert, *frag))
					return util::Result<Shader, Error>::success(*shader);
				else
					return util::Result<Shader, Error>::failure(shader.get_error());
			}
			else
				return util::Result<Shader, Error>::failure(Error::CANNOT_OPEN_FILE);
		}
	};
}

#endif
