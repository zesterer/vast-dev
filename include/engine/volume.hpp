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

		bool is_solid() const
		{
			return this->data != 0;
		}

		glm::vec4 get_color() const
		{
			const glm::vec4 colors[7] = {
				glm::vec4(0, 0, 0, 0),       // 0
				glm::vec4(1, 1, 1, 1),       // 1
				glm::vec4(1, 0.7, 0.7, 1),   // 2
				glm::vec4(0.7, 1, 0.7, 1),   // 3
				glm::vec4(0.7, 0.7, 1, 0.4), // 4
				glm::vec4(1, 1, 0.7, 1),     // 5
				glm::vec4(0.2, 0.2, 0.2, 1), // 6
			};

			return colors[this->data];
		}

		bool has_alpha() const
		{
			return this->get_color().a < 1.0f;
		}

		bool is_border_with(Voxel const& other)
		{
			if (this->is_solid())
			{
				if (this->has_alpha())
					return !other.is_solid();
				else
					return other.has_alpha();
			}
			else
				return false;
		}

		glm::vec2 get_uvs()
		{
			const glm::vec2 uvs[7] = {
				glm::vec2(0, 0), // 0
				glm::vec2(0, 1), // 1
				glm::vec2(1, 0), // 2
				glm::vec2(1, 1), // 3
				glm::vec2(0, 0), // 4
				glm::vec2(0, 0), // 5
				glm::vec2(0, 0), // 6
			};

			return uvs[this->data];
		}

		Voxel(int data = 0) : data(data) {}

		static Voxel empty()
		{
			return Voxel();
		}
	};

	enum class VolumeError { CANNOT_OPEN_FILE };

	// A component that encapsulates all volumetric structures
	// Examples: Spacecraft, space stations, houses, etc.
	struct Volume
	{
		core::ObjIdent ident;

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

		void queue_remesh()
		{
			this->rev ++;
		}

		void clear()
		{
			this->voxels.clear();
			this->queue_remesh();
		}

		void set_size(glm::ivec3 size)
		{
			this->size = size;
			this->voxels = std::vector<Voxel>(size.x * size.y * size.z);
			this->clear();
		}

		util::Status<VolumeError> load_from(std::string filename);

		Volume(core::ObjIdent ident, glm::ivec3 size) :
			ident(ident),
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
	template<> void Component<Volume>::add(Scene& scene, oid_t id);
	template<> void Component<Volume>::remove(Scene& scene, oid_t id);
	template<> void Component<Volume>::tick(Scene& scene, float dt);
}

#endif
