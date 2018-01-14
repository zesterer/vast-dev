#ifndef VAST_HPP_GFX_FIGURE
#define VAST_HPP_GFX_FIGURE

// Local
#include <core/cm.hpp>
#include <core/engine/entity.hpp>
#include <util/result.hpp>
#include <gfx/res/mesh.hpp>
#include <gfx/res/model.hpp>
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
	// A figure is any entity that has an associated 3D model, texture, and optional normal map
	struct Figure
	{
		glm::mat4 mat;
		res::Model model;
		Pipeline& pipeline;

		void update_from(std::shared_ptr<core::engine::Entity> entity)
		{
			this->mat = glm::translate(glm::mat4(1), entity->pos);
			this->mat *= glm::mat4_cast(entity->ori); // Should this be converted to matrix first?
			this->mat = glm::scale(this->mat, entity->scale);
		}

		Figure(Pipeline& pipeline) : pipeline(pipeline)
		{
			res::Mesh m;
			m.add(res::Poly(
				res::Vert(glm::vec3(1, 1, -0.5), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1), glm::vec2(0, 0)),
				res::Vert(glm::vec3(1, -1, -0.5), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1), glm::vec2(0, 0)),
				res::Vert(glm::vec3(-1, -1, -0.5), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1), glm::vec2(0, 0))
			));

			this->model = res::Model(m,
				gl::GL_TRIANGLES,
				{
					{ "vert_pos", FormatType::F32, 3 }, // Position
					{ "vert_color", FormatType::F32, 3 }, // Color
					{ "vert_norm", FormatType::F32, 3 }, // Normal
					{ "vert_uv", FormatType::F32, 2 }, // UVs
				}
			);

			// if (auto mesh = res::Mesh::from("data/mesh/craft.obj"))
			// 	this->model = res::Model(*mesh);
			// else
			// 	util::panic("Could not load mesh");

			std::cout << "Created Figure" << std::endl;
		}

		~Figure() { std::cout << "Deleted Figure" << std::endl; }
	};

	// Used to store the unique variant id for models
	extern int FIGURE_VARIANT_ID;

	// A box containing all figures
	extern core::ComponentBox<Figure> figures;

	// Possible errors when attempting to manipulate figures
	enum class FigureError { INVALID_ROOT, NOT_A_FIGURE };

	// Get a reference to a figure, given a component root and an object id
	util::Result<std::shared_ptr<Figure>, core::ComponentError> figure_get(core::ComponentRoot const& root, id_t id);

	// Create a new figure component
	void figure_create(core::ComponentRoot& root, id_t id);

	// Remove a figure component
	void figure_remove(core::ComponentRoot& root, id_t id);

	// Perform a tick on all figure components
	void figure_tick(core::ComponentRoot& root, float dt);

	// Create an instance describing the figure variant
	core::ComponentVariant figure_variant();
}

#endif
