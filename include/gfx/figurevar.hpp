#ifndef VAST_HPP_GFX_FIGUREVAR
#define VAST_HPP_GFX_FIGUREVAR

// Local
#include <core/cm.hpp>
#include <core/engine/entityvar.hpp>
#include <util/result.hpp>

// Std
#include <iostream>
#include <map>

namespace vast::gfx
{
	// A figure is any entity that has an associated 3D model, texture, and optional normal map
	struct Figure
	{
		Figure() { std::cout << "Created Figure" << std::endl; }
		~Figure() { std::cout << "Deleted Figure" << std::endl; }
	};

	// Used to store the unique variant id for models
	static int FIGURE_VARIANT_ID;


	// A box containing all figures
	core::ComponentBox<Figure> figures;

	// Possible errors when attempting to manipulate figures
	enum class FigureError { INVALID_ROOT, NOT_A_FIGURE };

	// Get a reference to a figure, given a component root and an object id
	util::Result<Figure&, core::ComponentBox<Figure>::Error> figure_get(core::ComponentRoot& root, id_t id)
	{
		return figures.get(root, id);
	}

	// Create a new figure component
	void figure_create(core::ComponentRoot& root, id_t id)
	{
		// Create a dependency components first
		core::engine::entity_create(root, id);

		figures.emplace(root, id);
	}

	// Remove a figure component
	void figure_remove(core::ComponentRoot& root, id_t id)
	{
		figures.remove(root, id);
	}

	// Perform a tick on a figure component
	void figure_tick(core::ComponentRoot& root, float dt)
	{
		(void)root;
		(void)dt;
		// TODO : Do figures need a tick component?
		//for (auto pair : figures[root.id])
		//	// Tick here
	}

	// Register the figure as a component variant
	__attribute__((constructor)) void register_figure_var()
	{
		FIGURE_VARIANT_ID = core::cm_register_component();
	}

	// Create an instance describing the figure variant
	core::ComponentVariant figure_variant()
	{
		return core::ComponentVariant(
			FIGURE_VARIANT_ID,
			&figure_create,
			&figure_remove,
			&figure_tick
		);
	}
}

#endif
