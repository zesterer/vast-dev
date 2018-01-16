// Local
#include <gfx/figure.hpp>
#include <gfx/pipeline.hpp>
#include <util/panic.hpp>

// Std
#include <optional>
#include <functional>

namespace vast::gfx
{
	// Used to store the unique variant id for models
	int FIGURE_VARIANT_ID;

	// A box containing all figures
	core::ComponentBox<Figure> figures;

	// Get a reference to a figure, given a component root and an object id
	util::Result<std::shared_ptr<Figure>, core::ComponentError> figure_get(core::ComponentRoot const& root, id_t id)
	{
		return figures.get(root, id);
	}

	// The pipeline and uniforms used for all figures
	static std::optional<Pipeline> figure_pipeline;
	static std::optional<Uniform>
		uni_time,
		uni_mmat,
		uni_vmat,
		uni_pmat;

	// Initiate the figure variant
	void figure_init(core::ComponentRoot& root)
	{
		(void)root;

		// Create the figure pipeline if needed
		if (!figure_pipeline)
		{
			if (auto shader = res::Shader::from_files("data/shader/vert.glsl", "data/shader/frag.glsl"))
				figure_pipeline = Pipeline(
					*shader,
					Target()
				);
			else
				util::panic("Could not create figure pipeline shader");

			// Find uniform indexes
			uni_time = figure_pipeline->get_uniform("uni_time").data_or(Uniform(-1));
			uni_mmat = figure_pipeline->get_uniform("uni_mmat").data_or(Uniform(-1));
			uni_vmat = figure_pipeline->get_uniform("uni_vmat").data_or(Uniform(-1));
			uni_pmat = figure_pipeline->get_uniform("uni_pmat").data_or(Uniform(-1));
		}
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
			if (auto entity = core::engine::entities.get(root, pair.first)) // Update figure from entity
				pair.second->update_from(*entity);
	}

	// Render all figures
	void render_figures(core::Scene const& scene, Camera const& cam)
	{
		if (!figure_pipeline) // Only continue if the figure pipeline has been configured
			return;

		// Set the pipeline up for figure rendering
		figure_pipeline->bind();
		figure_pipeline->set_uniform(*uni_time, scene.time);
		figure_pipeline->set_uniform(*uni_vmat, cam.vmat);
		figure_pipeline->set_uniform(*uni_pmat, cam.pmat);

		// Render each figure
		for (auto pair : figures.components(scene.croot))
			pair.second->render(*figure_pipeline);
	}

	// Create an instance describing the figure variant
	core::ComponentVariant figure_variant()
	{
		return core::ComponentVariant(
			FIGURE_VARIANT_ID,
			&figure_init,
			&figure_remove,
			&figure_tick
		);
	}

	// Render a single figure
	void Figure::render(Pipeline& pipeline)
	{
		pipeline.set_uniform(*uni_mmat, this->mat);

		this->model.bind();

		gl::glDrawArrays(this->model.gl_primitive, 0, this->model.vertex_count);
	}

	// Register the figure as a component variant
	__attribute__((constructor)) void register_figure_var()
	{
		FIGURE_VARIANT_ID = core::cm_register_component();
	}
}

namespace vast::core
{
	using namespace gfx;

	template <> Figure* Scene::get<Figure>(id_t id)
	{
		auto figure = figures.get(this->croot, id);
		if (figure)
			return &**figure;
		else
			return nullptr;
	}

	template <> id_t Scene::create<Figure>()
	{
		id_t id = this->create<engine::Entity>();

		if (figure_pipeline)
			figures.emplace(this->croot, id, *figure_pipeline);
		else
			util::panic("Creating Figure without creating pipeline");

		return id;
	}
}
