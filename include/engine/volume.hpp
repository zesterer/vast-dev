#ifndef VAST_HPP_ENGINE_VOLUME
#define VAST_HPP_ENGINE_VOLUME

// Local
#include <core/scene.hpp>

// Lib
#include <glm/glm.hpp>

// Std
#include <iostream>
#include <vector>

namespace vast::engine
{
	struct Voxel
	{
		int data;

		Voxel(int data = 0) : data(data) {}
	};

	struct Volume
	{
		glm::ivec3 size;
		glm::vec3 center;
		std::vector<Voxel> voxels;
		int rev = 0;

		void set(int x, int y, int z, Voxel vox)
		{
			this->voxels[
				x * this->size[2] * this->size[1] +
				y * this->size[1] +
				z
			] = vox;
		}

		Volume(glm::ivec3 size, glm::vec3 center = glm::vec3(0)) :
			size(size),
			center(center),
			voxels(size.x * size.y * size.z)
		{
			std::cout << "Created Volume" << std::endl;
		}

		~Volume() { std::cout << "Deleted Volume" << std::endl; }
	};
}

namespace vast::core
{
	using namespace engine;

	template<> void Component<Volume>::init(Scene& scene);
	template<> void Component<Volume>::add(Scene& scene, id_t id);
	template<> void Component<Volume>::remove(Scene& scene, id_t id);
	template<> void Component<Volume>::tick(Scene& scene, float dt);
}

#endif
