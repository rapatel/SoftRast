// Software-Based Triangle Rasterizer

#include <string>
#include <iostream>
#include <stdlib.h>

#include "main.h"


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
    int longestEdge = 0;
    float longestEdgeLen = 0;
    for(int i = 0; i < 3; i++)
    {
        float edgeLen = edges[i].m_V2.m_Y - edges[i].m_V1.m_Y;
        if(edgeLen > longestEdgeLen)
        {
            longestEdge = i;
            longestEdgeLen = edgeLen;
        }
    }
}

void TriRasterizer::drawSpansBetweenEdges(const Edge &e1, const Edge &e2)
{
}

void TriRasterizer::drawSpan(const Span &span, int y)
{
}

void main()
{
    return;
}