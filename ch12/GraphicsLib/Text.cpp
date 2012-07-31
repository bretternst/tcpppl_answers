#include "Text.h"
#include "Surface.h"

namespace GraphicsLib
{
	void Text::Draw(Surface& s) const
	{
		s.DrawText(p1, p2, str, Stroke());
	}
}
