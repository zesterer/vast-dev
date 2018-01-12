#ifndef VAST_HPP_GFX_SHADER
#define VAST_HPP_GFX_SHADER

// Local
#include <util/result.hpp>

// Lib
#include <glbinding/gl/types.h>

// Std
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>

namespace vast::gfx
{
	struct Shader
	{
		gl::GLuint gl_id;

		Shader() {}
		Shader(gl::GLuint vert_gl_id, gl::GLuint frag_gl_id)
		{
			this->gl_id = gl::glCreateProgram();
			gl::glAttachShader(this->gl_id, vert_gl_id);
			gl::glAttachShader(this->gl_id, frag_gl_id);
			gl::glLinkProgram(this->gl_id);
		}

		enum class Error { CANNOT_OPEN_FILE, COMPILE_ERROR };

		static util::Result<Shader, Error> from_strings(std::string const& vert_str, std::string const& frag_str)
		{
			auto create_shader = [](gl::GLenum type, std::string const& str) -> util::Result<gl::GLuint, std::string>
			{
				gl::GLuint gl_id = gl::glCreateShader(type);
				const char* src_buff = str.c_str();
				gl::glShaderSource(gl_id, 1, &src_buff, nullptr);
				gl::glCompileShader(gl_id);

				gl::GLint result, log_len;
				gl::glGetShaderiv(gl_id, gl::GL_COMPILE_STATUS, &result);
				gl::glGetShaderiv(gl_id, gl::GL_INFO_LOG_LENGTH, &log_len);
				if (log_len > 1)
				{
					std::vector<char> log_buff(log_len + 1);
					gl::glGetShaderInfoLog(gl_id, log_len, nullptr, &log_buff[0]);
					return util::Result<gl::GLuint, std::string>::failure(std::string(&log_buff[0]));
				}
				else
					return util::Result<gl::GLuint, std::string>::success(gl_id);
			};

			// Create the shaders
			auto vert_r = create_shader(gl::GL_VERTEX_SHADER, vert_str);
			auto frag_r = create_shader(gl::GL_FRAGMENT_SHADER, frag_str);
			if (vert_r.is_failure() || frag_r.is_failure())
				return util::Result<Shader, Error>::failure(Error::COMPILE_ERROR);
			else
				return util::Result<Shader, Error>::success(Shader(vert_r.get_data(), frag_r.get_data()));
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
			auto vert_r = read_file(vert_filename);
			auto frag_r = read_file(frag_filename);
			if (vert_r.is_failure() || frag_r.is_failure())
				return util::Result<Shader, Error>::failure(Error::CANNOT_OPEN_FILE);
			else
			{
				auto shader_r = Shader::from_strings(frag_r.get_data(), frag_r.get_data());
				if (shader_r.is_failure())
					return util::Result<Shader, Error>::failure(Error::COMPILE_ERROR);
				else
					return util::Result<Shader, Error>::success(shader_r.get_data());
			}
		}
	};
}

#endif
