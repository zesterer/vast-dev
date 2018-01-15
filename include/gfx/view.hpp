#ifndef VAST_HPP_GFX_VIEW
#define VAST_HPP_GFX_VIEW

namespace vast::gfx
{
	struct View
	{
		int width, height;

		View(int w, int h) : width(w), height(h) {}
	};
}

#endif
