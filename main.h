#ifndef MAIN_H
#define MAIN_H

#include <string>
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;

typedef unsigned char uchar;
typedef unsigned int uint;

class Vertex;
class Color;
class Edge;
class Span;
class TriRasterizer;
class Pixel;

#define WIDTH 1024
#define HEIGHT 1024

//-----------------------------------------------------------------------------
// Color class: Defines the color of a vertex
//-----------------------------------------------------------------------------
class Color
{
public:
    Color(uchar r = 0, uchar g = 0, uchar b = 0);
    ~Color();
public:
    void setColor(uchar r, uchar g, uchar b);
    uchar r, g, b;
};

Color::Color(uchar r, uchar g, uchar b) :
    r(r),
    g(g),
    b(b)
{
}

Color::~Color()
{
}

void Color::setColor(uchar r, uchar g, uchar b)
{
    this->r = r;
    this->g = g;
    this->b = b;
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
    void setPixelLoc(uint x_pos, uint y_pos);
    void setPixelCol(const Color &color);
public:
    Color m_color;
    uint m_x, m_y;
    float m_x_center, m_y_center; // center location
};

//  sets location of pixel
void Pixel::setPixelLoc(uint x_pos, uint y_pos)
{
    m_x = x_pos;
    m_y = y_pos;

    // keep track of exact center
    m_x_center = (float)x_pos + 0.5f;
    m_y_center = (float)y_pos + 0.5f;
}

// sets pixel color
void Pixel::setPixelCol(const Color &color)
{
    m_color = color;
}


//-----------------------------------------------------------------------------
// TriRasterizer class: rasterize a triangle
//-----------------------------------------------------------------------------
class TriRasterizer
{
public:
    TriRasterizer(uint w, uint h) :
        m_width(w),
        m_height(h)
    {
        m_pixels = new Pixel[m_width * m_height];
        for (uint i = 0; i < m_height; i++)
        {
            for (uint j = 0; j < m_width; j++)
            {
                m_pixels[i*m_width + j].setPixelLoc(j, i);
            }
        }
    }
    ~TriRasterizer()
    {
        delete[] m_pixels;
    }
    float getNextPixCenter(float val)
    {
        float tmp = ceil(val*2)/2;
        float intpart(0.f);
        float fractpart = modf(tmp, &intpart);

        return (fractpart == 0.5f) ? tmp : tmp + 0.5f;
    }
    void drawTriangle(const Vertex &v1, const Vertex &v2, const Vertex &v3);
    void drawSpansBetweenEdges(const Edge &e1, const Edge &e2);
    void drawSpan(const Span &span);
    void interpolateColor(float curP, float srcP,
                          float desP, const Color &srcC1,
                          const Color &desC2, Color &intC);
    void setPixel(uint x, uint y, const Color &color)
    {
        m_pixels[y*m_width + x].setPixelCol(color);
    }
    void fillPixels(uchar *pixels)
    {
        for (uint i = 0; i < m_height; i++)
        {
            for (uint j = 0; j < m_width; j++)
            {
                *pixels++ = m_pixels[i*m_width + j].m_color.r;
                *pixels++ = m_pixels[i*m_width + j].m_color.g;
                *pixels++ = m_pixels[i*m_width + j].m_color.b;
            }
        }
    }
private:
    uint m_width, m_height; // in pixels
    Pixel* m_pixels;
};


#endif
