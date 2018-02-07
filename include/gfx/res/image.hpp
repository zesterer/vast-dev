#ifndef VAST_HPP_GFX_RES_IMAGE
#define VAST_HPP_GFX_RES_IMAGE

// Local
#include <util/result.hpp>
#include <util/panic.hpp>

// Lib
#include <glm/glm.hpp>
#ifdef __MINGW32__
	#include <SOIL.h>
#else
	#include <SOIL/SOIL.h>
#endif

// Std
#include <string>

namespace vast::gfx::res
{
	enum class ImageError { CANNOT_OPEN_FILE };

	struct Image
	{
		std::vector<uint8_t> data;
		glm::ivec2 size;

		Image(const uint8_t* ptr, glm::ivec2 size) : size(size)
		{
			data.assign(ptr, ptr + size[0] * size[1] * 3);
		}

		static util::Result<Image, ImageError> from(std::string filename)
		{
			glm::ivec2 size;
			uint8_t* data = SOIL_load_image(filename.c_str(), &size.x, &size.y, 0, SOIL_LOAD_RGB);

			if (data == nullptr)
				return util::Result<Image, ImageError>::failure(ImageError::CANNOT_OPEN_FILE);

			Image img(data, size);

			std::free(data);

			return util::Result<Image, ImageError>::success(img);
		}
	};
}

#endif
