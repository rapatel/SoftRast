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
    int longEdge = 0;
    int shortEdge0 = 0;
    int shortEdge1 = 0;
    float longestEdgeLen = 0;
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

void TriRasterizer::drawSpansBetweenEdges(const Edge &e1, const Edge &e2)
{
    if(e2.m_V1.m_Y == e2.m_V2.m_Y) return; // horizontal edge, return

    float currY = getNextPixCenter(e2.m_V1.m_Y); // TODO: snap to next pixel center
    float topY = e2.m_V2.m_Y;
    while(currY < topY)
    {
        // TODO: calculate span points/colors
        currY = getNextPixCenter(e2.m_V1.m_Y);
    }
}

void TriRasterizer::drawSpan(const Span &span, int y)
{
}

void main()
{
    return;
}