#include "dot.h"
#include "surface.h"

namespace gfxlib
{
	void Dot::Draw(Surface& s) const
	{
		s.DrawDot(pos, Thickness(), Stroke());
	}
}
