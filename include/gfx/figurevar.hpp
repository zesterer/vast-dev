#ifndef VAST_HPP_GFX_FIGUREVAR
#define VAST_HPP_GFX_FIGUREVAR

// Local
#include <core/cm.hpp>
#include <core/engine/entityvar.hpp>
#include <util/result.hpp>
#include <gfx/res/mesh.hpp>
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
		res::Mesh mesh;
		Pipeline& pipeline;

		void update_from(std::shared_ptr<core::engine::Entity> entity)
		{
			this->mat = glm::translate(glm::mat4(1), entity->pos);
			this->mat *= glm::mat4_cast(entity->ori); // Should this be converted to matrix first?
			this->mat = glm::scale(this->mat, entity->scale);
		}

		Figure(Pipeline& pipeline) : pipeline(pipeline)
		{
			auto mesh_r = res::Mesh::from("data/mesh/craft.obj");
			if (mesh_r.is_success())
				this->mesh = mesh_r.get_data();
			else
				util::panic("Could not load mesh");

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
	util::Result<std::shared_ptr<Figure>, core::ComponentError> figure_get(core::ComponentRoot& root, id_t id);

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
