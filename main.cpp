// Software-Based Triangle Rasterizer
#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glut.h>

#include <string>
#include <iostream>
#include <stdlib.h>
#include "main.h"

using namespace std;

// globals
uchar* g_pixels;

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
    shortEdge0 = (longEdge+1) % 3;
    shortEdge1 = (longEdge+2) % 3;

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

    // get current colors
    Color currLongColor;
    Color currShortColor;

    // get inverse slopes
    float shortInvSlope = (shortEdge.m_V2.m_X == shortEdge.m_V1.m_X) ? 0.f :
        (shortEdge.m_V2.m_X-shortEdge.m_V1.m_X)/(shortEdge.m_V2.m_Y-shortEdge.m_V1.m_Y);
    float longInvSlope  = (longEdge.m_V2.m_X == longEdge.m_V1.m_X) ? 0.f :
        (longEdge.m_V2.m_X-longEdge.m_V1.m_X)/(longEdge.m_V2.m_Y-longEdge.m_V1.m_Y);

        // get current x-axis vals
    float currLongX  = longEdge.m_V1.m_X + (currY-longEdge.m_V1.m_Y) * longInvSlope;
    float currShortX = shortEdge.m_V1.m_X + (currY-shortEdge.m_V1.m_Y) * shortInvSlope;

    // move along edges
    Span span;
    while (currY < topY)
    {
        // TODO: only calc interpolatants once
        interpolateColor(currY, shortEdge.m_V1.m_Y, shortEdge.m_V2.m_Y,
            shortEdge.m_V1.m_Color, shortEdge.m_V2.m_Color, currShortColor);
        interpolateColor(currY, longEdge.m_V1.m_Y, longEdge.m_V2.m_Y,
            longEdge.m_V1.m_Color, longEdge.m_V2.m_Color, currLongColor);

        // set & draw span
        span.setSpan(currShortColor, currLongColor, currShortX, currLongX, currY);
        drawSpan(span);

        currY += 1.f;
        currShortX += shortInvSlope;
        currLongX += longInvSlope;
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

        // set pixel
        setPixel((uint)floor(currX), (uint)floor(span.y), currColor);

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
    intC.r = (uchar)(((float)srcC1.r)*(1-frac) + ((float)desC2.r)*frac);
    intC.g = (uchar)(((float)srcC1.g)*(1-frac) + ((float)desC2.g)*frac);
    intC.b = (uchar)(((float)srcC1.b)*(1-frac) + ((float)desC2.b)*frac);
}

void makeImage()
{
    // rasterize setup
    // Setup our rasterizer
    TriRasterizer rasterizer(WIDTH, HEIGHT);
    Vertex vertices[3];

    vertices[0].m_Color.setColor(255, 0, 0);
    vertices[0].m_X = 20;
    vertices[0].m_Y = 20;

    vertices[1].m_Color.setColor(0, 255, 0);
    vertices[1].m_X = 1000;
    vertices[1].m_Y = 100;

    vertices[2].m_Color.setColor(0, 0, 255);
    vertices[2].m_X = 400;
    vertices[2].m_Y = 1000;

    rasterizer.drawTriangle(vertices[0], vertices[1], vertices[2]);
    rasterizer.fillPixels(g_pixels);
}

void display()
{
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawPixels(WIDTH,HEIGHT,GL_RGB,GL_UNSIGNED_BYTE,g_pixels);
    glutSwapBuffers();
}

void init( ) {
    glViewport(0, 0, WIDTH, HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0., WIDTH, 0., HEIGHT);
    makeImage();
}

void reshape(int w, int h) {
}

void main(int argc, char** argv)
{
    g_pixels = new uchar[WIDTH*HEIGHT*3];
    memset(g_pixels, 0, WIDTH*HEIGHT*3*sizeof(uchar));

    // GLUT
    glutInit(&argc, argv);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(100, 100);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("TriangleRasterizer");

    init();
    glutDisplayFunc(display);
    //glutMouseFunc(onMousePress);
    //glutIdleFunc(onIdle);
    //glutKeyboardFunc(onKeyboard);
    //glutReshapeFunc(reshape);
    glutMainLoop();

    delete[] g_pixels;
    return;
}