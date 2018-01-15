#ifndef VAST_HPP_GFX_CAMERA
#define VAST_HPP_GFX_CAMERA

// Local
#include <gfx/view.hpp>
#include <core/engine/entity.hpp>

// Lib
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Std
#include <limits>

namespace vast::gfx
{
	struct Camera
	{
		glm::mat4 vmat, pmat;

		glm::vec3 _pos;
		glm::quat _ori;
		float _fov, _sr;

		void update()
		{
			glm::normalize(this->_ori);

			//this->mat = this->_ori * this->mat; // <--- Not this!

			this->vmat = glm::mat4(1);

			//this->vmat = glm::rotate(this->vmat, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0)); // Yaw

			// Transform to standard coordinate space
			//this->vmat = glm::rotate(this->vmat, glm::radians(-40.0f), glm::vec3(0.0, 1.0, 0.0));
			//this->vmat = glm::rotate(this->vmat, glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));

			this->vmat = glm::rotate(this->vmat, glm::radians(+90.0f), glm::vec3(0.0, 1.0, 0.0));
			this->vmat = glm::rotate(this->vmat, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));

			this->vmat = this->vmat * glm::inverse(glm::mat4_cast(this->_ori));
			this->vmat = glm::translate(this->vmat, -this->_pos);

			//this->vmat = glm::inverse(this->vmat);

			this->pmat = glm::perspective(glm::radians(this->_fov), this->_sr, std::numeric_limits<float>::min(), 1.0f);
		}

		void set_view(View const& view)
		{
			this->_sr = static_cast<float>(view.width) / view.height;
			this->update();
		}

		void set_fov(float fov)
		{
			this->_fov = fov;
			this->update();
		}

		void update_from(core::engine::Entity const& entity)
		{
			this->_pos = entity.pos;
			this->_ori = entity.ori;

			this->update();
		}

		Camera(float fov) :
			_pos(glm::vec3(0)),
			_ori(glm::quat()),
			_fov(fov),
			_sr(1.0f)
		{}
	};
}

#endif
