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
		int volume_rev = 0;

		void check_remesh(engine::Volume const& volume, Figure& figure)
		{
			if (this->volume_rev != volume.rev)
			{
				this->volume_rev = volume.rev;

				res::Mesh m;
				m.add({
					res::Poly(
						res::Vert(glm::vec3(1, -1, -0.5), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1), glm::vec2(0, 0)),
						res::Vert(glm::vec3(1, 1, -0.5), glm::vec3(1, 1, 0), glm::vec3(0, 0, 1), glm::vec2(0, 0)),
						res::Vert(glm::vec3(-1, -1, -0.5), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec2(0, 0))
					),
					res::Poly(
						res::Vert(glm::vec3(1, 1, -0.5), glm::vec3(1, 1, 0), glm::vec3(0, 0, 1), glm::vec2(0, 0)),
						res::Vert(glm::vec3(-1, 1, -0.5), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1), glm::vec2(0, 0)),
						res::Vert(glm::vec3(-1, -1, -0.5), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec2(0, 0))
					)
				});

				figure.model.set_mesh(m);
			}
		}

		Body(engine::Volume const& volume) : volume_rev(volume.rev - 1) // Incorrect rev will trigger remesh
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
	template<> void Component<Body>::add(Scene& scene, id_t id);
	template<> void Component<Body>::remove(Scene& scene, id_t id);
	template<> void Component<Body>::tick(Scene& scene, float dt);
}

#endif
