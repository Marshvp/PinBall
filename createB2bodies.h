#pragma once
#include "box2d/box2d.h"
#include "raylib.h"

extern const float PIXELS_PER_METER;
float px_to_m(float px);
float m_to_px(float m);

b2WorldId CreateWorld(float gravityY = 9.81f);

b2BodyId CreateDynamicBall(b2WorldId world, Vector2 center_px, float radius_px, float density = 1.0f, float friction = 0.2f, float restitution = 0.8f);

b2BodyId CreateStaticBox(b2WorldId world, Vector2 center_px, Vector2 size_px, float angleDeg = 0.0f, float friction = 0.3f , float restitution = 0.0f);

void CreateTableWalls(b2WorldId world, const Rectangle& table, float wallThickness_px = 12.0f);

b2BodyId CreateSlantObstacles(b2WorldId world, const Rectangle& slantRect, float tiltAngleDeg, float friction = 0.3f, float restition = 0.8f);