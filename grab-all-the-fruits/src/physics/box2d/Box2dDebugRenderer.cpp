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
    
    vector<Vertex> verticesVector;
    
    /*b2Vec2 p1 = vertices[vertexCount - 1];
	for (int32 i = 0; i < vertexCount; ++i)
	{
		b2Vec2 p2 = vertices[i];
		verticesVector.push_back(Vertex(Vector2(p1.x * METERS_PER_PIXEL, p1.y * METERS_PER_PIXEL), Color(color.r * 255, color.g * 255, color.b * 255, color.a * 255)));
        verticesVector.push_back(Vertex(Vector2(p2.x * METERS_PER_PIXEL, p2.y * METERS_PER_PIXEL), Color(color.r * 255, color.g * 255, color.b * 255, color.a * 255)));
		p1 = p2;
	}*/
    verticesVector.push_back(Vertex(Vector2(10, 10), Color(255, 0, 0, 255)));
    verticesVector.push_back(Vertex(Vector2(10, 15), Color(255, 255, 0, 255)));
    verticesVector.push_back(Vertex(Vector2(15, 15), Color(255, 255, 255, 255)));
    verticesVector.push_back(Vertex(Vector2(20, 15), Color(255, 255, 0, 255)));
    verticesVector.push_back(Vertex(Vector2(20, 20), Color(255, 0, 0, 255)));
    verticesVector.push_back(Vertex(Vector2(10, 10), Color(255, 0, 0, 255)));

    DrawCall drawCall;
    drawCall.vertices = verticesVector;
    m_pPlatformManager->renderTexture(drawCall);
}

void Box2dDebugRenderer::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
    vector<Vertex> verticesVector;
    
    b2Vec2 p1;
    for (int i = 0; i < vertexCount; i++)
	{
        if (i % 3 == 0 && i > 0)
        {
            p1 = vertices[i - 1];
            verticesVector.push_back(Vertex(Vector2(m_offset.x + p1.x * PIXELS_PER_METER, m_offset.y + m_worldSize.y - p1.y * PIXELS_PER_METER), Color(color.r * 255, color.g * 255, color.b * 255, color.a * 255)));
        }
        p1 = vertices[i];
        verticesVector.push_back(Vertex(Vector2(m_offset.x + p1.x * PIXELS_PER_METER, m_offset.y + m_worldSize.y - p1.y * PIXELS_PER_METER), Color(color.r * 255, color.g * 255, color.b * 255, color.a * 255)));
	}
    p1 = vertices[0];
    verticesVector.push_back(Vertex(Vector2(m_offset.x + p1.x * PIXELS_PER_METER, m_offset.y + m_worldSize.y - p1.y * PIXELS_PER_METER), Color(color.r * 255, color.g * 255, color.b * 255, color.a * 255)));
    
    DrawCall drawCall;
    drawCall.vertices = verticesVector;
    m_pPlatformManager->renderTexture(drawCall);
};

void Box2dDebugRenderer::DrawCircle(const b2Vec2& center, float radius, const b2Color& color) {};

void Box2dDebugRenderer::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) {};

void Box2dDebugRenderer::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {};

void Box2dDebugRenderer::DrawTransform(const b2Transform& xf) {};

void Box2dDebugRenderer::DrawPoint(const b2Vec2& p, float size, const b2Color& color) {};

void Box2dDebugRenderer::updateOffset(const Vector2& rOffset)
{
    m_offset = rOffset;
}