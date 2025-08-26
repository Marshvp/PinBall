#include "createB2bodies.h"
#include "raymath.h"


const float PIXELS_PER_METER = 50.0f;
float px_to_m(float px) { return px / PIXELS_PER_METER; };
float m_to_px(float m) { return m * PIXELS_PER_METER; };

b2WorldId CreateWorld(float gravityY) {
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = (b2Vec2){0.0f, gravityY};
    b2WorldId worldId = b2CreateWorld(&worldDef);
    return worldId;
}

b2BodyId CreateDynamicBall(b2WorldId world, Vector2 center_px, float radius_px, float density, float friction, float restitution) {

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = (b2Vec2){px_to_m(center_px.x), px_to_m(center_px.y)};
    b2BodyId bodyId = b2CreateBody(world, &bodyDef);

    b2Circle circle;
    circle.center = (b2Vec2){0.0f, 0.0f};
    circle.radius = px_to_m(radius_px);

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = density;
    shapeDef.friction = friction;
    shapeDef.restitution = restitution;

    b2CreateCircleShape(bodyId, &shapeDef, &circle);
    return bodyId;
}

b2BodyId CreateStaticBox(b2WorldId world, Vector2 center_px, Vector2 size_px, float angleDeg, float friction, float restitution) {

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.position = (b2Vec2){ px_to_m(center_px.x), px_to_m(center_px.y) };
    bodyDef.rotation = b2MakeRot(angleDeg * DEG2RAD);
    b2BodyId bodyId = b2CreateBody(world, &bodyDef);

    b2Polygon box = b2MakeBox(px_to_m(size_px.x * 0.5f), px_to_m(size_px.y * 0.5f));

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.friction = 0.3f;
    shapeDef.restitution = restitution;
    b2CreatePolygonShape(bodyId, &shapeDef, &box);

    return bodyId;
}


void CreateTableWalls(b2WorldId world, const Rectangle& table, float thickness_px) {
    Vector2 topCenter = { table.x + table.width*0.5f, table.y - thickness_px*0.5f };
    Vector2 bottomCenter = { table.x + table.width*0.5f, table.y + table.height - thickness_px*0.5f };
    Vector2 leftCenter = { table.x + thickness_px*0.5f, table.y + table.height*0.5f };
    Vector2 rightCenter = { table.x + table.width - thickness_px*0.5f, table.y + table.height*0.5f };

    Vector2 SizeH = { table.width, thickness_px };
    Vector2 SizeV = { thickness_px, table.height };

    CreateStaticBox(world, topCenter, SizeH, 0.0f, 0.3f, 0.2f);
    CreateStaticBox(world, bottomCenter, SizeH, 0.0f, 0.3f, 0.2f);
    CreateStaticBox(world, leftCenter, SizeV, 0.0f, 0.3f, 0.2f);
    CreateStaticBox(world, rightCenter, SizeV, 0.0f, 0.3f, 0.2f);
}

b2BodyId CreateSlantObstacles(b2WorldId world, const Rectangle& slantRect, float tiltAngleDeg, float friction, float restition) {
    return CreateStaticBox(world, {slantRect.x, slantRect.y },{ slantRect.width, slantRect.height }, tiltAngleDeg, friction, restition);
}