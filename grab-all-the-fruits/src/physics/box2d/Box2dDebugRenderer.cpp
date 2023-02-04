#include "Box2dDebugRenderer.hpp"
#include "platform/DrawCall.h"
#include "logic/GameConfiguration.h"
#include <vector>

using namespace std;

Box2dDebugRenderer::Box2dDebugRenderer(const Vector2& rWorldSize, PlatformManager* pPlatformManager) : b2Draw()
{
    initializeMembers();
    m_pPlatformManager = pPlatformManager;
    m_worldSize = rWorldSize;
}

Box2dDebugRenderer::~Box2dDebugRenderer()
{
    initializeMembers();
}

void Box2dDebugRenderer::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    vector<Vertex> linesVector;
    b2Vec2 p1 = vertices[vertexCount - 1];
    for (int i = 0; i < vertexCount; i++)
	{
        b2Vec2 p2 = vertices[i];
        linesVector.push_back(toVertex(p1, color));
        linesVector.push_back(toVertex(p2, color));
        p1 = p2;
	}
    
    DrawCall drawCall;
    drawCall.lines = linesVector;
    m_pPlatformManager->renderTexture(drawCall);
}

void Box2dDebugRenderer::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
    vector<Vertex> verticesVector;
    b2Color fillColor(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
    b2Vec2 p1;
    for (int i = 0; i < vertexCount; i++)
	{
        if (i % 3 == 0 && i > 0)
        {
            p1 = vertices[i - 1];
            verticesVector.push_back(toVertex(p1, fillColor));
        }
        p1 = vertices[i];
        verticesVector.push_back(toVertex(p1, fillColor));
	}
    p1 = vertices[0];
    verticesVector.push_back(toVertex(p1, fillColor));
    
    DrawCall drawCall;
    drawCall.vertices = verticesVector;
    m_pPlatformManager->renderTexture(drawCall);

    drawCall.vertices.clear();
    p1 = vertices[vertexCount - 1];
	for (int32 i = 0; i < vertexCount; ++i)
	{
		b2Vec2 p2 = vertices[i];
		drawCall.lines.push_back(toVertex(p1, color));
        drawCall.lines.push_back(toVertex(p2, color));
		p1 = p2;
	}
    m_pPlatformManager->renderTexture(drawCall);
};

void Box2dDebugRenderer::DrawCircle(const b2Vec2& center, float radius, const b2Color& color) 
{
    vector<Vertex> verticesVector;
    const float k_segments = 16.0f;
	const float k_increment = 2.0f * b2_pi / k_segments;
	float sinInc = sinf(k_increment);
	float cosInc = cosf(k_increment);
	b2Vec2 v0 = center;
	b2Vec2 r1(cosInc, sinInc);
	b2Vec2 v1 = center + radius * r1;
	b2Color fillColor(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
	for (int32 i = 0; i < k_segments; ++i)
	{
		// Perform rotation to avoid additional trigonometry.
		b2Vec2 r2;
		r2.x = cosInc * r1.x - sinInc * r1.y;
		r2.y = sinInc * r1.x + cosInc * r1.y;
		b2Vec2 v2 = center + radius * r2;
		
        verticesVector.push_back(toVertex(v0, fillColor));
        verticesVector.push_back(toVertex(v1, fillColor));
        verticesVector.push_back(toVertex(v2, fillColor));

		r1 = r2;
		v1 = v2;
	}

    DrawCall drawCallVertices;
    drawCallVertices.lines = verticesVector;
    m_pPlatformManager->renderTexture(drawCallVertices);
};

void Box2dDebugRenderer::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) 
{
    vector<Vertex> verticesVector;
    const float k_segments = 16.0f;
	const float k_increment = 2.0f * b2_pi / k_segments;
	float sinInc = sinf(k_increment);
	float cosInc = cosf(k_increment);
	b2Vec2 v0 = center;
	b2Vec2 r1(cosInc, sinInc);
	b2Vec2 v1 = center + radius * r1;
	b2Color fillColor(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
	for (int32 i = 0; i < k_segments; ++i)
	{
		// Perform rotation to avoid additional trigonometry.
		b2Vec2 r2;
		r2.x = cosInc * r1.x - sinInc * r1.y;
		r2.y = sinInc * r1.x + cosInc * r1.y;
		b2Vec2 v2 = center + radius * r2;
		
        verticesVector.push_back(toVertex(v0, fillColor));
        verticesVector.push_back(toVertex(v1, fillColor));
        verticesVector.push_back(toVertex(v2, fillColor));

		r1 = r2;
		v1 = v2;
	}

    DrawCall drawCallVertices;
    drawCallVertices.vertices = verticesVector;
    m_pPlatformManager->renderTexture(drawCallVertices);

    vector<Vertex> linesVector;
	r1.Set(1.0f, 0.0f);
	v1 = center + radius * r1;
	for (int32 i = 0; i < k_segments; ++i)
	{
		b2Vec2 r2;
		r2.x = cosInc * r1.x - sinInc * r1.y;
		r2.y = sinInc * r1.x + cosInc * r1.y;
		b2Vec2 v2 = center + radius * r2;
		linesVector.push_back(toVertex(v1, color));
		linesVector.push_back(toVertex(v2, color));
		r1 = r2;
		v1 = v2;
	}

    DrawCall drawCallLines;
    drawCallLines.lines = linesVector;
    m_pPlatformManager->renderTexture(drawCallLines);

    linesVector.clear();
	// Draw a line fixed in the circle to animate rotation.
	b2Vec2 p = center + radius * axis;
	
    linesVector.push_back(toVertex(center, color));
	linesVector.push_back(toVertex(p, color));
    drawCallLines.lines = linesVector;
    m_pPlatformManager->renderTexture(drawCallLines);
};

void Box2dDebugRenderer::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) 
{
    DrawCall drawCallLines;
    drawCallLines.lines.push_back(toVertex(p1, color));
	drawCallLines.lines.push_back(toVertex(p2, color));
    m_pPlatformManager->renderTexture(drawCallLines);
};

void Box2dDebugRenderer::DrawTransform(const b2Transform& xf) 
{
    DrawCall drawCallLines;
    
    const float k_axisScale = 0.4f;
	b2Color red(1.0f, 0.0f, 0.0f);
	b2Color green(0.0f, 1.0f, 0.0f);
	b2Vec2 p1 = xf.p, p2;

	drawCallLines.lines.push_back(toVertex(p1, red));
	p2 = p1 + k_axisScale * xf.q.GetXAxis();
	drawCallLines.lines.push_back(toVertex(p2, red));
    
	drawCallLines.lines.push_back(toVertex(p1, green));
	p2 = p1 + k_axisScale * xf.q.GetYAxis();
	drawCallLines.lines.push_back(toVertex(p2, green));

    m_pPlatformManager->renderTexture(drawCallLines);
};

void Box2dDebugRenderer::DrawPoint(const b2Vec2& p, float size, const b2Color& color) 
{
    DrawCall drawCallPoints;
    drawCallPoints.point = toVertex(p, color);
    m_pPlatformManager->renderTexture(drawCallPoints);
};

void Box2dDebugRenderer::updateOffset(const Vector2& rOffset)
{
    m_offset = rOffset;
}

Vector2 Box2dDebugRenderer::convertToVector2(const b2Vec2& rB2Vec2) const
{
    return Vector2(m_offset.x + rB2Vec2.x * PIXELS_PER_METER, m_offset.y + m_worldSize.y - rB2Vec2.y * PIXELS_PER_METER);
}

Color Box2dDebugRenderer::convertToColor(const b2Color& color) const
{
    return Color(color.r * 255, color.g * 255, color.b * 255, color.a * 255);
}

Vertex Box2dDebugRenderer::toVertex(const b2Vec2& rB2Vec2, const b2Color& color) const
{
    return Vertex(convertToVector2(rB2Vec2), convertToColor(color));
}
