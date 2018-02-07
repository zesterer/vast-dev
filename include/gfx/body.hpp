#ifndef VAST_HPP_GFX_BODY
#define VAST_HPP_GFX_BODY

// Local
#include <core/scene.hpp>
#include <engine/volume.hpp>
#include <gfx/figure.hpp>
#include <gfx/res/mesh.hpp>

// Lib
#include <glm/glm.hpp>

namespace vast::gfx
{
	struct Body
	{
		core::ObjIdent ident;

		int _volume_rev = 0;

		void update_mesh(engine::Volume const& volume, Figure& figure);

		Body(core::ObjIdent ident, engine::Volume const& volume) : ident(ident), _volume_rev(volume.rev - 1) // Incorrect rev will trigger remesh
		{
			std::cout << "Created Body" << std::endl;
		}

		~Body() { std::cout << "Deleted Body" << std::endl; }
	};
}

namespace vast::core
{
	using namespace gfx;

	template<> void Component<Body>::init(Scene& scene);
	template<> void Component<Body>::add(Scene& scene, oid_t id);
	template<> void Component<Body>::remove(Scene& scene, oid_t id);
	template<> void Component<Body>::tick(Scene& scene, float dt);
}

#endif
