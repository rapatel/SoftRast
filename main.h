#ifndef MAIN_H
#define MAIN_H

#include <string>
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;

typedef unsigned short ushort;
typedef unsigned int uint;

class Vertex;
class Color;
class Edge;
class Span;
class Window;
class TriRasterizer;
class Pixel;


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
    Span(Color &color1, Color &color2, float x1, float x2, float y);
    Span() :
        x1(0),
        x2(0),
        y(0)
    {
    }
    ~Span();
    void setSpan(Color &color1, Color &color2, float x1, float x2, float y);
public:
    Color color1, color2;
    float x1, x2, y;
};

Span::Span(Color &color1, Color &color2, float x1, float x2, float y)
{
    setSpan(color1, color2, x1, x2, y);
}

Span::~Span()
{
}

// sets the values of the span
void Span::setSpan(Color &color1, Color &color2, float x1, float x2, float y)
{
    this->y = y;

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


//-----------------------------------------------------------------------------
// Pixel class: represent a pixel
//-----------------------------------------------------------------------------
class Pixel
{
public:
    Pixel(float x = 0.f, float y = 0.f) :
        m_x(x),
        m_y(y)
    {
    }
    ~Pixel()
    {
    }
    void setCenterLoc(float x_pos, float y_pos);
public:
    Color m_color;
    float m_x, m_y;
};

//  sets center location of pixel
void Pixel::setCenterLoc(float x_pos, float y_pos)
{
    m_x = x_pos;
    m_y = y_pos;
}


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
        m_pixels = new Pixel[m_width * m_height];
        for (uint i = 0; i < m_height; i++)
        {
            for (uint j = 0; j < m_width; j++)
            {
                m_pixels[i*m_width + j].setCenterLoc(((float)j+0.5f), ((float)i+0.5f));
            }
        }
    }
    ~Window()
    {
        delete[] m_pixels;
    }
    void drawWindow();
public:
    uint m_width, m_height; // in pixels
    Pixel* m_pixels;
};

// traverses window top-down
void Window::drawWindow()
{
    for (uint i = m_height; i > 0;)
    {
        i--;
        for (uint j = 0; j < m_width; j++)
        {
            printf("(%.2f,%.2f)", m_pixels[i*m_width + j].m_x, m_pixels[i*m_width + j].m_y);
        }
        cout << endl;
    }
}

//-----------------------------------------------------------------------------
// TriRasterizer class: rasterize a triangle
//-----------------------------------------------------------------------------
class TriRasterizer
{
public:
    TriRasterizer(uint w, uint h)
    {
        m_window = new Window(w, h);
    }
    ~TriRasterizer()
    {
        delete[] m_window;
    }
    float getNextPixCenter(float val)
    {
        float tmp = ceil(val*2)/2;
        float intpart(0.f);
        float fractpart = modf (tmp, &intpart);

        return (fractpart == 0.5f) ? tmp : tmp + 0.5f;
    }
    void drawTriangle(const Vertex &v1, const Vertex &v2, const Vertex &v3);
    void drawSpansBetweenEdges(const Edge &e1, const Edge &e2);
    void drawSpan(const Span &span);
public:
    Window* m_window;
};


#endif
