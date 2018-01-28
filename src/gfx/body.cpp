// Local
#include <gfx/body.hpp>

// Std
#include <functional>

namespace vast::gfx
{
	const int
		DIR_LEFT = 0,
		DIR_RIGHT = 1,
		DIR_BACK = 2,
		DIR_FRONT = 3,
		DIR_BOTTOM = 4,
		DIR_TOP = 5
	;

	// fmt = {origin, +x vec, +y vec, norm)
	glm::ivec3 side_vec[6][4] = {
		// left/right
		{ glm::ivec3(0, 1, 0), glm::vec3(+0, -1, +0), glm::vec3(+0, +0, +1), glm::vec3(-1, +0, +0) },
		{ glm::ivec3(1, 0, 0), glm::vec3(+0, +1, +0), glm::vec3(+0, +0, +1), glm::vec3(+1, +0, +0) },

		// back/front
		{ glm::ivec3(0, 0, 0), glm::vec3(+1, +0, +0), glm::vec3(+0, +0, +1), glm::vec3(+0, -1, +0) },
		{ glm::ivec3(1, 1, 0), glm::vec3(-1, +0, +0), glm::vec3(+0, +0, +1), glm::vec3(+0, +1, +0) },

		// bottom/top
		{ glm::ivec3(0, 1, 0), glm::vec3(+1, +0, +0), glm::vec3(+0, -1, +0), glm::vec3(+0, +0, +1) },
		{ glm::ivec3(0, 0, 1), glm::vec3(+1, +0, +0), glm::vec3(+0, +1, +0), glm::vec3(+0, +0, -1) },
	};

	res::Quad create_face(glm::ivec3 pos, int dir, glm::vec3 col)
	{
		glm::ivec3 origin = pos + side_vec[dir][0];
		glm::vec3 norm = side_vec[dir][3];
		return res::Quad(
			res::Vert(origin, col, norm, glm::vec2(0, 1)),
			res::Vert(origin + side_vec[dir][1], col, norm, glm::vec2(1, 1)),
			res::Vert(origin + side_vec[dir][2], col, norm, glm::vec2(0, 0)),
			res::Vert(origin + side_vec[dir][1] + side_vec[dir][2], col, norm, glm::vec2(1, 0))
		);
	}

	void Body::update_mesh(engine::Volume const& volume, Figure& figure)
	{
		if (this->_volume_rev != volume.rev)
		{
			this->_volume_rev = volume.rev;

			res::Mesh mesh;

			for (int i = 0; i < volume.size.x; i ++)
				for (int j = 0; j < volume.size.y; j ++)
					for (int k = 0; k < volume.size.z; k ++)
						if (volume.get(i, j, k).solid())
						{

							glm::ivec3 pos = glm::ivec3(i, j, k);
							glm::vec3 col = glm::vec3(1);

							if (!volume.get(i - 1, j, k).solid()) // left
								mesh.add(create_face(pos, DIR_LEFT, col));
							if (!volume.get(i + 1, j, k).solid()) // right
								mesh.add(create_face(pos, DIR_RIGHT, col));
							if (!volume.get(i, j - 1, k).solid()) // back
								mesh.add(create_face(pos, DIR_BACK, col));
							if (!volume.get(i, j + 1, k).solid()) // front
								mesh.add(create_face(pos, DIR_FRONT, col));
							if (!volume.get(i, j, k - 1).solid()) // bottom
								mesh.add(create_face(pos, DIR_BOTTOM, col));
							if (!volume.get(i, j, k + 1).solid()) // top
								mesh.add(create_face(pos, DIR_TOP, col));
						}

			figure.model.set_mesh(mesh);

			// res::Mesh m;
			// m.add({
			// 	res::Poly(
			// 		res::Vert(glm::vec3(1, -1, -0.5), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1), glm::vec2(0, 0)),
			// 		res::Vert(glm::vec3(1, 1, -0.5), glm::vec3(1, 1, 0), glm::vec3(0, 0, 1), glm::vec2(0, 0)),
			// 		res::Vert(glm::vec3(-1, -1, -0.5), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec2(0, 0))
			// 	),
			// 	res::Poly(
			// 		res::Vert(glm::vec3(1, 1, -0.5), glm::vec3(1, 1, 0), glm::vec3(0, 0, 1), glm::vec2(0, 0)),
			// 		res::Vert(glm::vec3(-1, 1, -0.5), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1), glm::vec2(0, 0)),
			// 		res::Vert(glm::vec3(-1, -1, -0.5), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec2(0, 0))
			// 	)
			// });
            //
			// figure.model.set_mesh(m);
		}
	}
}

namespace vast::core
{
	using namespace gfx;

	template<> ComponentBox<Body> Component<Body>::box = ComponentBox<Body>();

	template<> void Component<Body>::init(Scene& scene)
	{
		(void)scene;

		std::cout << "Body component initiated!" << std::endl;
	}

	template<> void Component<Body>::add(Scene& scene, id_t id)
	{
		// Add a volume and figure first
		Component<engine::Volume>::add(scene, id);
		Component<Figure>::add(scene, id);

		if (auto volume = scene.get<engine::Volume>(id))
			self::box.emplace(scene, id, *volume);
	}

	template<> void Component<Body>::tick(Scene& scene, float dt)
	{
		(void)dt;

		for (auto body : self::box.items(scene))
			if (auto volume = scene.get<engine::Volume>(body.first))
				if (auto figure = scene.get<Figure>(body.first))
					body.second->update_mesh(*volume, *figure);
	}

	template<> void Component<Body>::remove(Scene& scene, id_t id)
	{
		self::box.remove(scene, id);
	}
}
