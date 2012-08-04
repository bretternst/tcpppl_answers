#include "text.h"
#include "window.h"

namespace gfxlib
{
    void Text::draw(Window& s) const
    {
        s.draw_text(m_p1, m_p2, m_str, stroke());
    }
}
