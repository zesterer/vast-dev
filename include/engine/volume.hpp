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

		bool solid()
		{
			return this->data != 0;
		}

		Voxel(int data = 0) : data(data) {}

		static Voxel empty()
		{
			return Voxel();
		}
	};

	// A component that encapsulates all volumetric structures
	// Examples: Spacecraft, space stations, houses, etc.
	struct Volume
	{
		glm::ivec3 size;
		std::vector<Voxel> voxels;
		int rev = 0;

		Voxel get(int x, int y, int z) const
		{
			if (
				x < 0 || y < 0 || z < 0 ||
				x >= this->size.x || y >= this->size.y || z >= this->size.z
			)
				return Voxel::empty();

			return this->voxels[
				x * this->size.y * this->size.z +
				y * this->size.z +
				z
			];
		}

		void set(int x, int y, int z, Voxel vox)
		{
			if (
				x >= 0 && y >= 0 && z >= 0 &&
				x < this->size.x && y < this->size.y && z < this->size.z
			)
				this->voxels[
					x * this->size.y * this->size.z +
					y * this->size.z +
					z
				] = vox;
		}

		Volume(glm::ivec3 size) :
			size(size),
			voxels(size.x * size.y * size.z)
		{
			std::cout << "Created Volume of size (" << this->size.x << "," << this->size.y << "," << this->size.z << ")" << std::endl;
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
