// Local
#include <engine/vessel.hpp>
#include <engine/entity.hpp>
#include <engine/volume.hpp>
#include <util/panic.hpp>

namespace vast::engine
{
	bool Vessel::check_collision(phys::AABB3D box)
	{
		//box.pos += this->center;
		if (auto volume = this->ident.scene.get<Volume>(this->ident.id))
			for (float i = box.pos.x; i <= box.pos.x + box.size.x; i += 0.1f)
				for (float j = box.pos.y; j <= box.pos.y + box.size.y; j += 0.1f)
					for (float k = box.pos.z; k <= box.pos.z + box.size.z; k += 0.1f)
						if (volume->get(
							i + this->center.x,// - box.size.x / 2,
							j + this->center.y,// - box.size.y / 2,
							k + this->center.z// - box.size.z / 2
						).is_solid())
							return true;
		return false;
	}

	std::pair<phys::AABB3D, glm::vec3> Vessel::resolve_collision(phys::AABB3D box, glm::vec3 delta_pos)
	{
		if (this->ident.scene.get<Volume>(this->ident.id) != nullptr)
		{
			glm::vec3 initial = box.pos;

			glm::vec3 offs[3] = {
				glm::vec3(0, 0, 1),
				glm::vec3(0, 1, 0),
				glm::vec3(1, 0, 0),
			};

			glm::vec3 diff = delta_pos;
			for (size_t j = 0; j < 100; j ++)
				for (size_t i = 0; i < 3; i ++)
					if (!this->check_collision(phys::AABB3D(box.pos + diff * offs[i] * 0.01f, box.size)))
						box.pos += diff * offs[i] * 0.01f;

			return { box, box.pos - initial };
		}
		else
		{
			box.pos += delta_pos;
			return { box, delta_pos };
		}
	}
}

namespace vast::core
{
	using namespace engine;

	template<> ComponentBox<Vessel> Component<Vessel>::box = ComponentBox<Vessel>();

	template<> void Component<Vessel>::init(Scene& scene)
	{
		(void)scene;

		std::cout << "Vessel component initiated!" << std::endl;
	}

	template<> void Component<Vessel>::add(Scene& scene, oid_t id)
	{
		// Add entity and volume first
		Component<Entity>::add(scene, id);
		Component<Volume>::add(scene, id);

		if (auto volume = scene.get<Volume>(id))
			if (!volume->load_from("data/vox/cargoship.vox"))
				util::panic("Could not load voxel file");

		self::box.emplace(scene, id, ObjIdent(id, scene));
	}

	template<> void Component<Vessel>::tick(Scene& scene, float dt)
	{
		for (auto vessel : self::box.items(scene))
			vessel.second->tick(dt);
	}

	template<> void Component<Vessel>::remove(Scene& scene, oid_t id)
	{
		self::box.remove(scene, id);
	}
}
