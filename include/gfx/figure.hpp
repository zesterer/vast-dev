#ifndef VAST_HPP_GFX_FIGURE
#define VAST_HPP_GFX_FIGURE

// Local
#include <core/scene.hpp>
#include <engine/entity.hpp>
#include <util/result.hpp>
#include <gfx/res/mesh.hpp>
#include <gfx/res/model.hpp>
#include <gfx/res/image.hpp>
#include <gfx/res/texture.hpp>
#include <gfx/camera.hpp>
#include <gfx/pipeline.hpp>

// Std
#include <iostream>
#include <map>
#include <memory>

namespace vast::gfx
{
	extern std::optional<Pipeline> figure_pipeline;

	const size_t FIGURE_LAYERS = 2;
	const size_t FIGURE_OPAQUE_LAYER = 0;
	const size_t FIGURE_ALPHA_LAYER = 1;

	struct Figure
	{
		core::ObjIdent ident;

		res::Model layers[FIGURE_LAYERS];
		res::Texture texture;

		void render(engine::Entity const& entity, Pipeline& pipeline, size_t layer);

		Figure(core::ObjIdent ident) : ident(ident)
		{
			// if (auto mesh = res::Mesh::from("data/mesh/craft.obj"))
			// 	this->opaque_model = res::Model(
			// 		pipeline,
			// 		*mesh,
			// 		gl::GL_TRIANGLES,
			// 		{
			// 			{ "vert_pos", FormatType::F32, 3 }, // Position
			// 			{ "vert_color", FormatType::F32, 4 }, // Color
			// 			{ "vert_norm", FormatType::F32, 3 }, // Normal
			// 			{ "vert_uv", FormatType::F32, 2 }, // UVs
			// 		}
			// 	);
			// else
			// 	util::panic("Could not load mesh");

			if (auto image = res::Image::from("data/gfx/block.png"))
				this->texture = res::Texture(*image);
			else
				util::panic("Could not load image");

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
	template<> void Component<Figure>::add(Scene& scene, oid_t id);
	template<> void Component<Figure>::remove(Scene& scene, oid_t id);
	template<> void Component<Figure>::tick(Scene& scene, float dt);
}

#endif
