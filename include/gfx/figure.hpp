#ifndef VAST_HPP_GFX_FIGURE
#define VAST_HPP_GFX_FIGURE

// Local
#include <core/scene.hpp>
#include <engine/entity.hpp>
#include <util/result.hpp>
#include <gfx/res/mesh.hpp>
#include <gfx/res/model.hpp>
#include <gfx/camera.hpp>
#include <gfx/pipeline.hpp>

// Lib
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Std
#include <iostream>
#include <map>
#include <memory>

namespace vast::gfx
{
	struct Figure
	{
		glm::mat4 mat;
		res::Model model;

		void update_from(engine::Entity& entity)
		{
			this->mat = glm::translate(glm::mat4(1), entity.pos);
			this->mat *= glm::mat4_cast(entity.ori); // TODO: Should this be converted to matrix first?
			this->mat = glm::scale(this->mat, entity.scale);
		}

		void render(Pipeline& pipeline);

		Figure(Pipeline& pipeline)
		{
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
			// this->model = res::Model(
			// 	pipeline,
			// 	m,
			// 	gl::GL_TRIANGLES,
			// 	{
			// 		{ "vert_pos", FormatType::F32, 3 }, // Position
			// 		{ "vert_color", FormatType::F32, 3 }, // Color
			// 		{ "vert_norm", FormatType::F32, 3 }, // Normal
			// 		{ "vert_uv", FormatType::F32, 2 }, // UVs
			// 	}
			// );

			if (auto mesh = res::Mesh::from("data/mesh/craft.obj"))
				this->model = res::Model(
					pipeline,
					*mesh,
					gl::GL_TRIANGLES,
					{
						{ "vert_pos", FormatType::F32, 3 }, // Position
						{ "vert_color", FormatType::F32, 3 }, // Color
						{ "vert_norm", FormatType::F32, 3 }, // Normal
						{ "vert_uv", FormatType::F32, 2 }, // UVs
					}
				);
			else
				util::panic("Could not load mesh");

			std::cout << "Created Figure" << std::endl;
		}

		~Figure() { std::cout << "Deleted Figure" << std::endl; }
	};

	// Render all figures
	void render_figures(core::Scene const& scene, Camera const& cam);
}

namespace vast::core
{
	using namespace gfx;

	template<> void Component<Figure>::init(Scene& scene);
	template<> void Component<Figure>::add(Scene& scene, id_t id);
	template<> void Component<Figure>::remove(Scene& scene, id_t id);
	template<> void Component<Figure>::tick(Scene& scene, float dt);
}

#endif
