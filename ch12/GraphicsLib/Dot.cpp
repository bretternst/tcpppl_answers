#include "Dot.h"
#include "Surface.h"

namespace GraphicsLib
{
	void Dot::Draw(Surface& s) const
	{
		s.DrawDot(pos, Thickness(), Stroke());
	}
}
