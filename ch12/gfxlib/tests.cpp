#include "tests.h"
#include "window.h"
#include "dot.h"
#include "line.h"
#include "rect.h"
#include "circle.h"
#include "text.h"

int NumberOfWindows;

using namespace gfxlib;

void Exercise2();

void RunTests()
{
	Window* w = new Window(640, 480, 0xFFF0F0FF);

	// House structure
	Rect h(Point(200,400),Point(400,250));
	h.Stroke(0xFF483D8B);
	h.Fill(0xFF8B4513);
	w->Draw(&h);

	// Roof
	w->Draw(&Line(Point(200,250),Point(300,175)));
	w->Draw(&Line(Point(300,175),Point(400,250)));

	// Overhang
	w->Draw(&Line(Point(200,250),Point(190,257)));
	w->Draw(&Line(Point(400,250),Point(410,257)));

	// Door
	w->Draw(&Rect(Point(280,400),Point(320,340)));
	w->Draw(&Dot(Point(310,370)));

	// Windows
	Rect w1 = Rect(Point(230,380),Point(270,340));
	w->Draw(&w1);
	Line l1(w1.W(), w1.E());
	Line l2(w1.N(), w1.S());
	l1.Thickness(1);
	l2.Thickness(1);
	w->Draw(&l1);
	w->Draw(&l2);

	Rect w2(Point(330,380),Point(370,340));
	w->Draw(&w2);
	l1 = Line(w2.W(), w2.E());
	l2 = Line(w2.N(), w2.S());
	l1.Thickness(1);
	l2.Thickness(1);
	w->Draw(&l1);
	w->Draw(&l2);

	// Attic window
	Circle w3(Point(300, 225), 16);
	w->Draw(&w3);
	l1 = Line(w3.W(), w3.E());
	l2 = Line(w3.N(), w3.S());
	l1.Thickness(1);
	l2.Thickness(1);
	w->Draw(&l1);
	w->Draw(&l2);

	// Full moon
	Circle m(Point(550,75), 35);
	m.Thickness(1);
	m.Outline(Line::LINE_DOTTED);
	m.Stroke(0xFFFFFF00);
	m.Fill(0xFF40E0D0);
	w->Draw(&m);

	// Ground
	l1 = Line(Point(0,400),Point(640,400));
	l1.Thickness(1);
	l1.Outline(Line::LINE_DASHED);
	w->Draw(&l1);
	l1 = Line(Point(0,410),Point(640,410));
	l1.Thickness(1);
	l1.Outline(Line::LINE_DOTTED);
	w->Draw(&l1);

	// Test arrowhead
	l1 = Line(Point(50,50),Point(250,50));
	l2 = Line(Point(50,100),Point(250,100));
	l1.Thickness(1);
	l2.Thickness(1);
	l1.Arrowhead(Line::ARROW_START, Line::ARROW_IN);
	l1.Arrowhead(Line::ARROW_END, Line::ARROW_IN);
	l2.Arrowhead(Line::ARROW_START, Line::ARROW_OUT);
	l2.Arrowhead(Line::ARROW_END, Line::ARROW_OUT);
	w->Draw(&l1);
	w->Draw(&l2);

	l2 = Line(Point(275,150),Point(375,175));
	l2.Thickness(2);
	l2.Arrowhead(Line::ARROW_START, Line::ARROW_IN);
	l2.Arrowhead(Line::ARROW_END, Line::ARROW_OUT);
	l2.Stroke(0xff00ff00);
	w->Draw(&l2);

	// Test text
	Text t1(Point(50,20),Point(350,35),L"Hello world!");
	t1.Stroke(0xffff0000);
	w->Draw(&t1);

	// Test connect
	l1 = l1.Connect(&l2);
	l1.Thickness(1);
	w->Draw(&l1);
}
