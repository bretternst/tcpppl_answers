#include "text.h"
#include "surface.h"

namespace gfxlib
{
	void Text::Draw(Surface& s) const
	{
		s.DrawText(p1, p2, str, Stroke());
	}
}
