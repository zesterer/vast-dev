// Local
#include <engine/volume.hpp>

// Std
#include <cstdio>
#include <fstream>

namespace vast::engine
{
	util::Status<VolumeError> Volume::load_from(std::string filename)
	{
		this->clear();

		std::ifstream file(filename);
		if (file.is_open())
		{
			// Read volume size
			glm::ivec3 size;
			file >> size.x;
			file >> size.y;
			file >> size.z;

			// Read volume offset
			glm::vec3 offset;
			file >> offset.x;
			file >> offset.y;
			file >> offset.z;

			this->set_size(size);

			for (size_t i = 0; !file.eof() && i < static_cast<size_t>(this->size.x * this->size.y * this->size.z); i ++)
			{
				int vox;
				file >> vox;

				this->set(
					(i / (this->size.y * this->size.z)) % this->size.x,
					(i / this->size.z) % this->size.y,
					i % this->size.z,
					Voxel(vox)
				);
			}

			file.close();

			this->queue_remesh();

			return util::Status<VolumeError>::success();
		}
		else
			return util::Status<VolumeError>::failure(VolumeError::CANNOT_OPEN_FILE);
	}
}

namespace vast::core
{
	using namespace engine;

	template<> ComponentBox<Volume> Component<Volume>::box = ComponentBox<Volume>();

	template<> void Component<Volume>::init(Scene& scene)
	{
		(void)scene;

		std::cout << "Volume component initiated!" << std::endl;
	}

	template<> void Component<Volume>::add(Scene& scene, oid_t id)
	{
		auto volume = self::box.emplace(scene, id, ObjIdent(id, scene), glm::ivec3(15, 15, 15));

		// Randomise volume
		for (int i = 0; i < volume->size.x; i ++)
			for (int j = 0; j < volume->size.y; j ++)
				for (int k = 0; k < volume->size.z; k ++)
					volume->set(i, j, k, Voxel(rand() % 3 == 0));
	}

	template<> void Component<Volume>::tick(Scene& scene, float dt)
	{
		(void)scene;
		(void)dt;

		// Nothing yet
	}

	template<> void Component<Volume>::remove(Scene& scene, oid_t id)
	{
		self::box.remove(scene, id);
	}
}
