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
	static std::optional<Pipeline> figure_pipeline;
	static std::optional<Uniform>
		uni_time,
		uni_mmat,
		uni_vmat,
		uni_pmat;

	// Render a single figure
	void Figure::render(Pipeline& pipeline)
	{
		pipeline.set_uniform(*uni_mmat, this->mat);

		this->model.bind();

		gl::glDrawArrays(this->model.gl_primitive, 0, this->model.vertex_count);
	}

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
		for (auto figure : core::Component<Figure>::box.items(scene))
			figure.second->render(*figure_pipeline);
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
		}

		std::cout << "Figure component initiated!" << std::endl;
	}

	template<> id_t Component<Figure>::create(Scene& scene)
	{
		// Create an entity first
		id_t nid = scene.create<engine::Entity>();

		if (figure_pipeline)
			self::box.emplace(scene, nid, *figure_pipeline);
		else
			util::panic("Creating Figure without creating pipeline");

		return nid;
	}

	template<> void Component<Figure>::tick(Scene& scene, float dt)
	{
		(void)dt;

		for (auto figure : self::box.items(scene))
			if (auto entity = scene.get<engine::Entity>(figure.first))
				figure.second->update_from(*entity);
	}

	template<> void Component<Figure>::remove(Scene& scene, id_t id)
	{
		self::box.remove(scene, id);
	}
}
