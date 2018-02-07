// Local
#include <gfx/figure.hpp>
#include <gfx/pipeline.hpp>
#include <util/panic.hpp>

// Std
#include <optional>
#include <functional>

namespace vast::gfx
{
	// The pipeline and uniforms used for all figures
	std::optional<Pipeline> figure_pipeline;
	static std::optional<Uniform>
		uni_time,
		uni_mmat,
		uni_vmat,
		uni_pmat,
		uni_tex;

	// Render a single figure
	void Figure::render(engine::Entity const& entity, Pipeline& pipeline, size_t layer)
	{
		// Don't try to render if the layer isn't ready / doesn't exist
		if (!this->layers[layer].is_valid())
			return;

		pipeline.set_uniform(*uni_mmat, entity.mat);
		pipeline.bind_texture(*uni_tex, this->texture);

		this->layers[layer].bind();

		gl::glDrawArrays(this->layers[layer].gl_primitive, 0, this->layers[layer].vertex_count);
	}

	void render_figures(core::Scene& scene, Camera const& cam)
	{
		if (!figure_pipeline) // Only continue if the figure pipeline has been configured
			return;

		// Set the pipeline up for figure rendering
		figure_pipeline->bind();
		figure_pipeline->set_uniform(*uni_time, scene.time);
		figure_pipeline->set_uniform(*uni_vmat, cam.vmat);
		figure_pipeline->set_uniform(*uni_pmat, cam.pmat);

		// Render each figure
		for (size_t layer = 0; layer < FIGURE_LAYERS; layer ++)
			for (auto figure : core::Component<Figure>::box.items(scene))
				if (auto entity = scene.get<engine::Entity>(figure.first))
					figure.second->render(*entity, *figure_pipeline, layer);
	}
}

namespace vast::core
{
	using namespace gfx;

	template<> ComponentBox<Figure> Component<Figure>::box = ComponentBox<Figure>();

	template<> void Component<Figure>::init(Scene& scene)
	{
		(void)scene;

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
			uni_tex = figure_pipeline->get_uniform("uni_tex").data_or(Uniform(-1));
		}

		std::cout << "Figure component initiated!" << std::endl;
	}

	template<> void Component<Figure>::add(Scene& scene, oid_t id)
	{
		// Add an entity first
		Component<engine::Entity>::add(scene, id);

		if (figure_pipeline)
			self::box.emplace(scene, id, ObjIdent(id, scene));
		else
			util::panic("Creating Figure without creating pipeline");
	}

	template<> void Component<Figure>::tick(Scene& scene, float dt)
	{
		(void)scene;
		(void)dt;
	}

	template<> void Component<Figure>::remove(Scene& scene, oid_t id)
	{
		self::box.remove(scene, id);
	}
}
