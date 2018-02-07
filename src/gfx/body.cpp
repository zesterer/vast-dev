// Local
#include <gfx/body.hpp>
#include <gfx/figure.hpp>
#include <engine/volume.hpp>
#include <engine/vessel.hpp>

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
	glm::vec3 side_vec[6][4] = {
		// left/right
		{ glm::vec3(0, 1, 0), glm::vec3(+0, -1, +0), glm::vec3(+0, +0, +1), glm::vec3(-1, +0, +0) },
		{ glm::vec3(1, 0, 0), glm::vec3(+0, +1, +0), glm::vec3(+0, +0, +1), glm::vec3(+1, +0, +0) },

		// back/front
		{ glm::vec3(0, 0, 0), glm::vec3(+1, +0, +0), glm::vec3(+0, +0, +1), glm::vec3(+0, -1, +0) },
		{ glm::vec3(1, 1, 0), glm::vec3(-1, +0, +0), glm::vec3(+0, +0, +1), glm::vec3(+0, +1, +0) },

		// bottom/top
		{ glm::vec3(0, 1, 0), glm::vec3(+1, +0, +0), glm::vec3(+0, -1, +0), glm::vec3(+0, +0, -1) },
		{ glm::vec3(0, 0, 1), glm::vec3(+1, +0, +0), glm::vec3(+0, +1, +0), glm::vec3(+0, +0, +1) },
	};

	res::Quad create_face(glm::vec3 centre, glm::vec3 pos, int dir, glm::vec4 col, glm::vec2 block_uv)
	{
		glm::vec3 origin = pos - centre + side_vec[dir][0];
		glm::vec3 norm = side_vec[dir][3];
		float texmap_size = 2.0f;
		block_uv /= texmap_size;
		return res::Quad(
			res::Vert(origin, col, norm, block_uv + glm::vec2(0, 1) / texmap_size),
			res::Vert(origin + side_vec[dir][1], col, norm, block_uv + glm::vec2(1, 1) / texmap_size),
			res::Vert(origin + side_vec[dir][2], col, norm, block_uv + glm::vec2(0, 0) / texmap_size),
			res::Vert(origin + side_vec[dir][1] + side_vec[dir][2], col, norm, block_uv + glm::vec2(1, 0) / texmap_size)
		);
	}

	void Body::update_mesh(engine::Volume const& volume, Figure& figure)
	{
		if (this->_volume_rev != volume.rev)
		{
			this->_volume_rev = volume.rev;

			res::Mesh layer_meshes[FIGURE_LAYERS];

			glm::vec3 center = glm::vec3(0);
			if (auto vessel = this->ident.scene.get<engine::Vessel>(this->ident.id))
			{
				// Tmp
				vessel->center = glm::vec3(volume.size) / 2.0f;
				center = vessel->center;
			}

			for (int i = 0; i < volume.size.x; i ++)
				for (int j = 0; j < volume.size.y; j ++)
					for (int k = 0; k < volume.size.z; k ++)
					{
						auto vox = volume.get(i, j, k);
						if (vox.is_solid())
						{
							glm::vec3 pos = glm::vec3(i, j, k);
							glm::vec4 col = vox.get_color();
							glm::vec2 uvs = vox.get_uvs();

							res::Mesh& mesh = layer_meshes[vox.has_alpha() ? FIGURE_ALPHA_LAYER : FIGURE_OPAQUE_LAYER];

							if (vox.is_border_with(volume.get(i - 1, j, k))) // left
								mesh.add(create_face(center, pos, DIR_LEFT, col, uvs));
							if (vox.is_border_with(volume.get(i + 1, j, k))) // right
								mesh.add(create_face(center, pos, DIR_RIGHT, col, uvs));
							if (vox.is_border_with(volume.get(i, j - 1, k))) // back
								mesh.add(create_face(center, pos, DIR_BACK, col, uvs));
							if (vox.is_border_with(volume.get(i, j + 1, k))) // front
								mesh.add(create_face(center, pos, DIR_FRONT, col, uvs));
							if (vox.is_border_with(volume.get(i, j, k - 1))) // bottom
								mesh.add(create_face(center, pos, DIR_BOTTOM, col, uvs));
							if (vox.is_border_with(volume.get(i, j, k + 1))) // top
								mesh.add(create_face(center, pos, DIR_TOP, col, uvs));
						}
					}

			figure.layers[FIGURE_OPAQUE_LAYER].set_mesh(layer_meshes[FIGURE_OPAQUE_LAYER]);
			figure.layers[FIGURE_ALPHA_LAYER].set_mesh(layer_meshes[FIGURE_ALPHA_LAYER]);

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

	template<> void Component<Body>::add(Scene& scene, oid_t id)
	{
		// Add a volume and figure first
		Component<engine::Volume>::add(scene, id);
		Component<Figure>::add(scene, id);

		if (auto volume = scene.get<engine::Volume>(id))
			self::box.emplace(scene, id, ObjIdent(id, scene), *volume);

		if (auto figure = scene.get<Figure>(id))
		{
			if (auto mesh = res::Mesh::from("data/mesh/craft.obj"))
			{
				for (size_t layer = 0; layer < FIGURE_LAYERS; layer ++)
					figure->layers[layer] = res::Model(
						*figure_pipeline,
						*mesh,
						gl::GL_TRIANGLES,
						{
							{ "vert_pos", FormatType::F32, 3 }, // Position
							{ "vert_color", FormatType::F32, 4 }, // Color
							{ "vert_norm", FormatType::F32, 3 }, // Normal
							{ "vert_uv", FormatType::F32, 2 }, // UVs
						}
					);
			}
			else
				util::panic("Could not load mesh");
		}
	}

	template<> void Component<Body>::tick(Scene& scene, float dt)
	{
		(void)dt;

		for (auto body : self::box.items(scene))
			if (auto volume = scene.get<engine::Volume>(body.first))
				if (auto figure = scene.get<Figure>(body.first))
					body.second->update_mesh(*volume, *figure);
	}

	template<> void Component<Body>::remove(Scene& scene, oid_t id)
	{
		self::box.remove(scene, id);
	}
}
