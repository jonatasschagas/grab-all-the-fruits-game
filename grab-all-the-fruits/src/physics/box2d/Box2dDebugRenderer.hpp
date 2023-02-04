#pragma once
#ifndef Box2dDebugRenderer_hpp 
#define Box2dDebugRenderer_hpp

#include <box2d/box2d.h>
#include "platform/PlatformManager.h"
#include "core/Vector2.h"
#include "platform/DrawCall.h"

class Box2dDebugRenderer : public b2Draw
{
public:

    Box2dDebugRenderer(const Vector2& rWorldSize, PlatformManager* pPlatformManager);
    ~Box2dDebugRenderer();

    void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

	void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) override;

	void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) override;

	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;

	void DrawTransform(const b2Transform& xf) override;

	void DrawPoint(const b2Vec2& p, float size, const b2Color& color) override;
    
    void updateOffset(const Vector2& rOffset);

private:

    Vector2 convertToVector2(const b2Vec2& rB2Vec2) const;
    Color convertToColor(const b2Color& color) const;
    Vertex toVertex(const b2Vec2& rB2Vec2, const b2Color& color) const;

    PlatformManager* m_pPlatformManager;
    Vector2 m_worldSize;
    Vector2 m_offset;

    void initializeMembers()
    {
        m_pPlatformManager = nullptr;
    }

};

#endif /* Box2dDebugRenderer_hpp */
