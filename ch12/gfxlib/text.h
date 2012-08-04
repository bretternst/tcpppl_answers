#ifndef __TEXT_H__
#define __TEXT_H__

#include <string>

#include "shape.h"
#include "point.h"

namespace gfxlib
{
	class Text : public Shape
	{
	private:
		Point m_p1;
		Point m_p2;
		std::string m_str;

	public:
		Text(const Point& p1, const Point& p2, const std::string str) : m_p1(p1), m_p2(p2), m_str(str) {}
		virtual void draw(Window&) const;
		virtual const Point n() const { return Point(c().x(),m_p1.y()<m_p2.y()?m_p1.y():m_p2.y()); }
		virtual const Point s() const { return Point(c().x(),m_p1.y()>m_p2.y()?m_p1.y():m_p2.y()); }
		virtual const Point w() const { return Point(m_p1.x()<m_p2.x()?m_p1.x():m_p2.x(),c().y()); }
		virtual const Point e() const { return Point(m_p1.x()>m_p2.x()?m_p1.x():m_p2.x(),c().y()); }
		virtual const Point nw() const { return Point(w().x(),n().y()); }
		virtual const Point ne() const { return Point(e().x(),n().y()); }
		virtual const Point sw() const { return Point(w().x(),s().y()); }
		virtual const Point se() const { return Point(e().x(),s().y()); }
		virtual const Point c() const { return Point((m_p1.x()+m_p2.x())/2, (m_p1.y()+m_p2.y())/2); }
	};
}

#endif
