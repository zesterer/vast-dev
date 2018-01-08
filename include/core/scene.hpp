#ifndef VAST_HPP_CORE_SCENE
#define VAST_HPP_CORE_SCENE

namespace vast::core
{
	struct Scene
	{
		float time;

		Scene() : time(0.0f) {}

		void tick(float dt)
		{
			time += dt;
		}
	};
}

#endif
