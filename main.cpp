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

        // TODO: only calc interpolatants once
        interpolateColor(currY, shortEdge.m_V1.m_Y, shortEdge.m_V2.m_Y,
            shortEdge.m_V1.m_Color, shortEdge.m_V2.m_Color, currShortColor);
        interpolateColor(currY, longEdge.m_V1.m_Y, longEdge.m_V2.m_Y,
            longEdge.m_V1.m_Color, longEdge.m_V2.m_Color, currLongColor);

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
    Color currColor;
    while (currX <= span.x2)
    {
        // TODO: only calc interpolate once
        interpolateColor(currX, span.x1, span.x2,
            span.color1, span.color2, currColor);

        // TODO: draw to screen
        //drawPixel(currX, span.y,);

        currX += 1.f;
    }
}

void TriRasterizer::interpolateColor(float curP, float srcP,
                                     float desP, const Color &srcC1,
                                     const Color &desC2, Color &intC)
{
    // calculate frac of distance traveled
    float frac = abs((curP-srcP)/(desP-srcP));

    // interpolate color
    intC.r = (ushort)(((float)srcC1.r)*(1-frac) + ((float)desC2.r)*frac);
    intC.g = (ushort)(((float)srcC1.g)*(1-frac) + ((float)desC2.g)*frac);
    intC.b = (ushort)(((float)srcC1.b)*(1-frac) + ((float)desC2.b)*frac);
}

void main()
{
    return;
}