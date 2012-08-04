#include "dot.h"
#include "window.h"

namespace gfxlib
{
	void Dot::draw(Window& s) const
	{
		s.draw_dot(pos, thickness(), stroke());
	}
}
