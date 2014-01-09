#ifndef MAIN_H
#define MAIN_H

typedef unsigned short ushort;
typedef unsigned int uint;

class Vertex;
class Color;
class Edge;
class Span;
class TriRasterizer;
class Window;


//-----------------------------------------------------------------------------
// Color class: Defines the color of a vertex
//-----------------------------------------------------------------------------
class Color
{
public:
    Color(ushort r = 0, ushort g = 0, ushort b = 0);
    ~Color();
public:
    ushort r, g, b;
};

Color::Color(ushort r, ushort g, ushort b) :
    r(r),
    g(g),
    b(b)
{
}

Color::~Color()
{
}

//-----------------------------------------------------------------------------
// Vertex class: Defines a vertex
//-----------------------------------------------------------------------------
class Vertex
{
public:
    Vertex(const Color &color, float x = 0.f, float y = 0.f) :
        m_Color(color),
        m_X(x),
        m_Y(y)
    {
    }
    Vertex() :
        m_X(0),
        m_Y(0)
    {
    }
    ~Vertex()
    {
    }
public:
    float m_X, m_Y; // position
    Color m_Color; // color
};


//-----------------------------------------------------------------------------
// Edge class: Defines the edge of a triangle
//-----------------------------------------------------------------------------
class Edge
{
public:
    Edge(const Vertex &v1, const Vertex &v2);
    ~Edge();
public:
    Vertex m_V1, m_V2;
};

Edge::Edge(const Vertex &v1, const Vertex &v2)
{
    // V1 lower than V2 (y-axis)
    if (v1.m_Y <= v2.m_Y)
    {
        m_V1 = v1;
        m_V2 = v2;
    }
    else
    {
        m_V1 = v2;
        m_V2 = v1;
    }
}

Edge::~Edge()
{
}


//-----------------------------------------------------------------------------
// Span class: Defines the horizontal span of pixels
//-----------------------------------------------------------------------------
class Span
{
public:
    Span(Color &color1, Color &color2, float x1, float x2);
    ~Span();
public:
    Color color1, color2;
    float x1, x2;
};

Span::Span(Color &color1, Color &color2, float x1, float x2)
{
    // left-most point = point1
    if (x1 <= x2)
    {
        this->x1 = x1;
        this->color1 = color1;

        this->x2 = x2;
        this->color2 = color2;
    }
    else
    {
        this->x2 = x1;
        this->color2 = color1;

        this->x1 = x2;
        this->color1 = color2;
    }
}

Span::~Span()
{
}


//-----------------------------------------------------------------------------
// TriRasterizer class: rasterize a triangle
//-----------------------------------------------------------------------------
class TriRasterizer
{
public:
    TriRasterizer()
    {
    }
    ~TriRasterizer()
    {
    }
public:
    void drawTriangle(const Vertex &v1, const Vertex &v2, const Vertex &v3);
    void drawSpansBetweenEdges(const Edge &e1, const Edge &e2);
    void drawSpan(const Span &span, int y);
};


//-----------------------------------------------------------------------------
// Window class: represent window coordinates
//-----------------------------------------------------------------------------
class Window
{
public:
    Window(uint w, uint h) :
        m_width(w),
        m_height(h)
    {
        m_window = new Color[m_width * m_height];
    }
    ~Window()
    {
        delete[] m_window;
    }
public:
    uint m_width, m_height; // in pixels
    Color* m_window;
};

#endif