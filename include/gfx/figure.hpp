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

// Std
#include <iostream>
#include <map>
#include <memory>

namespace vast::gfx
{
	struct Figure
	{
		res::Model model;

		void render(engine::Entity const& entity, Pipeline& pipeline);

		Figure(Pipeline& pipeline)
		{
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
	void render_figures(core::Scene& scene, Camera const& cam);
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
