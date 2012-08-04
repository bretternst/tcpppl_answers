#include "tests.h"
#include "dot.h"
#include "line.h"
#include "rect.h"
#include "circle.h"
#include "text.h"

using namespace gfxlib;

void run_tests(gfxlib::Window *w)
{
	// House structure
	Rect h(Point(200,400),Point(400,250));
	h.stroke(0xFF483D8B);
	h.fill(0xFF8B4513);
	w->draw(&h);

	// Roof
    Line l(Point(200,250), Point(300,175));
    w->draw(&l);
    l = Line(Point(300,175),Point(400,250));
    w->draw(&l);

	// Overhang
    l = Line(Point(200, 250), Point(190,257));
    w->draw(&l);
    l = Line(Point(400,250),Point(410,257));
    w->draw(&l);

	// Door
    Rect r(Point(280,400),Point(320,340));
    w->draw(&r);
    Dot d(Point(310,370));
    w->draw(&d);

	// Windows
	Rect w1 = Rect(Point(230,380),Point(270,340));
	w->draw(&w1);
	Line l1(w1.w(), w1.e());
	Line l2(w1.n(), w1.s());
	l1.thickness(1);
	l2.thickness(1);
	w->draw(&l1);
	w->draw(&l2);

	Rect w2(Point(330,380),Point(370,340));
	w->draw(&w2);
	l1 = Line(w2.w(), w2.e());
	l2 = Line(w2.n(), w2.s());
	l1.thickness(1);
	l2.thickness(1);
	w->draw(&l1);
	w->draw(&l2);

	// Attic window
	Circle w3(Point(300, 225), 16);
	w->draw(&w3);
	l1 = Line(w3.w(), w3.e());
	l2 = Line(w3.n(), w3.s());
	l1.thickness(1);
	l2.thickness(1);
	w->draw(&l1);
	w->draw(&l2);

	// Full moon
	Circle m(Point(550,75), 35);
	m.thickness(1);
	m.outline(Line::LINE_DOTTED);
	m.stroke(0xFFFFFF00);
	m.fill(0xFF40E0D0);
	w->draw(&m);

	// Ground
	l1 = Line(Point(0,400),Point(640,400));
	l1.thickness(1);
	l1.outline(Line::LINE_DASHED);
	w->draw(&l1);
	l1 = Line(Point(0,410),Point(640,410));
	l1.thickness(1);
	l1.outline(Line::LINE_DOTTED);
	w->draw(&l1);

	// Test arrowhead
	l1 = Line(Point(50,50),Point(250,50));
	l2 = Line(Point(50,100),Point(250,100));
	l1.thickness(1);
	l2.thickness(1);
	l1.arrowhead(Line::ARROW_START, Line::ARROW_IN);
	l1.arrowhead(Line::ARROW_END, Line::ARROW_IN);
	l2.arrowhead(Line::ARROW_START, Line::ARROW_OUT);
	l2.arrowhead(Line::ARROW_END, Line::ARROW_OUT);
	w->draw(&l1);
	w->draw(&l2);

	l2 = Line(Point(275,150),Point(375,175));
	l2.thickness(2);
	l2.arrowhead(Line::ARROW_START, Line::ARROW_IN);
	l2.arrowhead(Line::ARROW_END, Line::ARROW_OUT);
	l2.stroke(0xff00ff00);
	w->draw(&l2);

	// Test connect
	l1 = l1.connect(&l2);
	l1.thickness(1);
	w->draw(&l1);

	// Test text
	Text t1(Point(50,10),Point(350,50),"Hello world!");
	t1.stroke(0xffff0000);
	w->draw(&t1);

    w->paint();
}
