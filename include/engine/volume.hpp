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
