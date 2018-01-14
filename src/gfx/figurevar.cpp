// Local
#include <gfx/figurevar.hpp>
#include <gfx/pipeline.hpp>
#include <util/panic.hpp>

// Std
#include <optional>

namespace vast::gfx
{
	// Used to store the unique variant id for models
	int FIGURE_VARIANT_ID;

	// A box containing all figures
	core::ComponentBox<Figure> figures;

	// Get a reference to a figure, given a component root and an object id
	util::Result<std::shared_ptr<Figure>, core::ComponentError> figure_get(core::ComponentRoot& root, id_t id)
	{
		return figures.get(root, id);
	}

	// The pipeline used for all figures by default
	static bool pipeline_initiated = false;
	std::optional<Pipeline> figure_pipeline;

	// Initiate the figure variant
	void figure_init(core::ComponentRoot& root)
	{
		(void)root;

		// Create the figure pipeline if needed
		if (!pipeline_initiated)
		{
			auto shader_r = res::Shader::from_files("data/shader/vert.glsl", "data/shader/frag.glsl");

			if (shader_r.is_failure())
				util::panic("Could not create figure pipeline shader");

			figure_pipeline = Pipeline(
				res::Format({
					{res::FormatType::F32, 3}, // Position
					{res::FormatType::F32, 3}, // Color
					{res::FormatType::F32, 3}, // Normal
					{res::FormatType::F32, 2}, // UVs
				}),
				shader_r.get_data(),
				Target()
			);
			pipeline_initiated = true;
		}
	}

	// Create a new figure component
	void figure_create(core::ComponentRoot& root, id_t id)
	{
		// Create a dependency components first
		core::engine::entity_create(root, id);

		if (figure_pipeline)
			figures.emplace(root, id, *figure_pipeline);
		else
			util::panic("Tried to initiate Figure without first creating figure pipeline");
	}

	// Remove a figure component
	void figure_remove(core::ComponentRoot& root, id_t id)
	{
		figures.remove(root, id);
	}

	// Perform a tick on all figure components
	void figure_tick(core::ComponentRoot& root, float dt)
	{
		(void)dt;

		for (auto pair : figures.components(root))
		{
			Figure& figure = *pair.second;

			// Update the figure component matrices from the entity component
			auto r = core::engine::entity_get(root, pair.first);
			if (r.is_success())
				figure.update_from(r.get_data());
		}
	}

	// Create an instance describing the figure variant
	core::ComponentVariant figure_variant()
	{
		return core::ComponentVariant(
			FIGURE_VARIANT_ID,
			&figure_init,
			&figure_create,
			&figure_remove,
			&figure_tick
		);
	}

	// Register the figure as a component variant
	__attribute__((constructor)) void register_figure_var()
	{
		FIGURE_VARIANT_ID = core::cm_register_component();
	}
}
