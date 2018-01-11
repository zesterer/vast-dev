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

	};

	// Used to store the unique variant id for models
	static int FIGURE_VARIANT_ID;

	// A hashmap containing all components of the figure variant (TODO: Replace the std::map with a more performant cache-friendly structure)
	std::map<core::cm_id, std::map<id_t, Figure>> figures;

	// Possible errors when attempting to manipulate figures
	enum class FigureError { INVALID_ROOT, NOT_A_FIGURE };

	// Get a reference to a figure, given a component root and an object id
	util::Result<Figure&, FigureError> figure_get(core::ComponentRoot& root, id_t id)
	{
		// Have we yet registed this component root?
		auto it0 = figures.find(root.id);
		if (it0 == figures.end())
			return util::Result<Figure&, FigureError>::failure(FigureError::INVALID_ROOT);

		// Have we registered this id as a figure?
		auto it1 = it0->second.find(id);
		if (it1 == it0->second.end())
			return util::Result<Figure&, FigureError>::failure(FigureError::NOT_A_FIGURE);

		return util::Result<Figure&, FigureError>::success(it1->second);
	}

	// Create a new figure component
	void figure_create(core::ComponentRoot& root, id_t id)
	{
		// Create an entity component first
		core::engine::entity_create(root, id);

		figures[root.id].emplace(id, Figure());
		std::cout << "Created figure!" << std::endl;
	}

	// Remove a figure component
	void figure_remove(core::ComponentRoot& root, id_t id)
	{
		auto it = figures[root.id].find(id);
		if (it != figures[root.id].end())
		{
			figures[root.id].erase(it);
			std::cout << "Removed figure!" << std::endl;
		}
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
