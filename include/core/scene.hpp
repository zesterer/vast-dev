#ifndef VAST_HPP_CORE_SCENE
#define VAST_HPP_CORE_SCENE

// Local
#include <core/cm.hpp>

// Lib
#include <glm/glm.hpp>

// Std
#include <vector>
#include <memory>
#include <functional>

namespace vast::core
{
	struct Scene
	{
		float time;
		ComponentRoot croot;

		id_t cam;

		template <typename T> T* get(id_t id);
		template <typename T> id_t create();

		void tick(float dt);
		void setup();
		void clear();

		Scene() : time(0.0f) {}

		~Scene()
		{
			this->clear();
		}
	};
}

#endif
