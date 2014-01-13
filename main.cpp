// Software-Based Triangle Rasterizer

#include <string>
#include <iostream>
#include <stdlib.h>
#include "main.h"

using namespace std;

void TriRasterizer::drawTriangle(const Vertex &v1,
                                 const Vertex &v2,
                                 const Vertex &v3)
{
    // create 3 edges
    Edge edges[3] = {
        Edge(v1, v2),
        Edge(v2, v3),
        Edge(v3, v1)
    };

    // find tallest edge (y-axis)
    int longEdge            = 0;
    int shortEdge0          = 0;
    int shortEdge1          = 0;
    float longestEdgeLen    = 0;
    for(int i = 0; i < 3; i++)
    {
        float edgeLen = edges[i].m_V2.m_Y - edges[i].m_V1.m_Y;
        if(edgeLen > longestEdgeLen)
        {
            longEdge = i;
            longestEdgeLen = edgeLen;
        }
    }
    shortEdge0 = longEdge+1 % 3;
    shortEdge1 = longEdge+2 % 3;

    // draw spans between edges
    drawSpansBetweenEdges(edges[longEdge], edges[shortEdge0]);
    drawSpansBetweenEdges(edges[longEdge], edges[shortEdge1]);
}

void TriRasterizer::drawSpansBetweenEdges(const Edge &longEdge, const Edge &shortEdge)
{
    if(shortEdge.m_V1.m_Y == shortEdge.m_V2.m_Y) return; // horizontal edge, return

    // snap current y-axis val to next pixel center
    // using short edge as the reference top
    float currY = getNextPixCenter(shortEdge.m_V1.m_Y);
    float topY  = shortEdge.m_V2.m_Y;

    // get current x-axis vals
    float currLongX  = longEdge.m_V1.m_X;
    float currShortX = shortEdge.m_V1.m_X;

    // get current colors
    Color currLongColor;
    Color currShortColor;

    // get inverse slopes
    float shortInvSlope = (shortEdge.m_V2.m_X == shortEdge.m_V1.m_X) ? 0.f :
        (shortEdge.m_V2.m_X-shortEdge.m_V1.m_X)/(shortEdge.m_V2.m_Y-shortEdge.m_V1.m_Y);
    float longInvSlope  = (longEdge.m_V2.m_X == longEdge.m_V1.m_X) ? 0.f :
        (longEdge.m_V2.m_X-longEdge.m_V1.m_X)/(longEdge.m_V2.m_Y-longEdge.m_V1.m_Y);

    // move along edges
    Span span;
    while (currY < topY)
    {
        // interpolate x vals, colors
        currShortX  += (currY-shortEdge.m_V1.m_Y) * shortInvSlope;
        currLongX   += (currY-longEdge.m_V1.m_Y) * longInvSlope;

        interpolateColor(currShortX, currY, shortEdge, currShortColor);
        interpolateColor(currLongX, currY, shortEdge, currLongColor);

        // set & draw span
        span.setSpan(currShortColor, currLongColor, currShortX, currLongX, currY);
        drawSpan(span);

        currY += 1.f;
    }
}

void TriRasterizer::drawSpan(const Span &span)
{
    // snap current x-axis val to next pixel center
    // moving right
    float currX = getNextPixCenter(span.x1);

    // move horizontal along span
    while (currX <= span.x2)
    {
        //interpolateColor();
        //drawPixel(currX, span.y,);
        //currX += 1.f;
    }
}

void TriRasterizer::interpolateColor(const float x,
                                     const float y,
                                     const Edge &e,
                                     Color &c)
{
    // interpolate final color of a point on an edge
    float frac = abs((y-e.m_V1.m_Y)/(e.m_V2.m_Y-e.m_V2.m_Y));
    c.r = (ushort)(((float)e.m_V1.m_Color.r)*(1-frac) + ((float)e.m_V2.m_Color.r)*frac);
    c.g = (ushort)(((float)e.m_V1.m_Color.g)*(1-frac) + ((float)e.m_V2.m_Color.g)*frac);
    c.b = (ushort)(((float)e.m_V1.m_Color.b)*(1-frac) + ((float)e.m_V2.m_Color.b)*frac);
}

void main()
{
    return;
}