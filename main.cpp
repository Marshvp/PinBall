#include "raylib.h"
#include "raymath.h"
#include "math.h"
#include "box2d/box2d.h"
#include "createB2bodies.h"



struct Ball
{
    Vector2 center;
    float radius;
    Vector2 velocity;

};

struct Obstacle
{
    Rectangle rect;
    float bounceFactor;
    float tiltAngle;
};



Vector2 SlantCenterA(const Obstacle& slant) {
    float a = slant.tiltAngle * DEG2RAD;
    Vector2 center = { slant.rect.x, slant.rect.y };
    Vector2 direction = { cosf(a), sinf(a) };
    return Vector2Subtract(center, Vector2Scale(direction, slant.rect.width * 0.5f));
}
Vector2 SlantCenterB(const Obstacle& slant) {
    float a = slant.tiltAngle * DEG2RAD;
    Vector2 center = { slant.rect.x, slant.rect.y };
    Vector2 direction = { cosf(a), sinf(a) };
    return Vector2Add(center, Vector2Scale(direction, slant.rect.width * 0.5f));
}



int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "PinBall Game");
    SetTargetFPS(60);

    Ball ball = { {140, 80}, 10.0f, {120.0f, 0.0f}};
    Rectangle table = { 100, 50, 600, 500 };

    b2WorldId worldId = CreateWorld(9.81f);



    b2BodyId ballBody = CreateDynamicBall(worldId, ball.center, ball.radius, 1.0f, 0.02f, 0.92f);

    CreateTableWalls(worldId, table);


    ////
    ////

    const float Obstacle_W = 120.0f;
    const float Obstacle_H = 16.0f;
    float halfW_m = px_to_m(Obstacle_W/2);
    float halfH_m = px_to_m(Obstacle_H/2);


    float ObstacleCenterY = table.y + table.height - 150.0f + Obstacle_H/2;

    Obstacle slant1 = { 
        {table.x + 40.0f + Obstacle_W/2.0f, ObstacleCenterY, Obstacle_W, Obstacle_H}, 
        0, 
        -25.0f
    };
    Obstacle slant2 = { 
        {table.x + table.width - 40.0f - Obstacle_W/2.0f, ObstacleCenterY, Obstacle_W, Obstacle_H},
        0, 
        25.0f
    };
    
    ////

    b2BodyId slantBody1 = CreateSlantObstacles(worldId, slant1.rect, slant1.tiltAngle, 0.3f, 0.8f);
    b2BodyId slantBody2 = CreateSlantObstacles(worldId, slant2.rect, slant2.tiltAngle, 0.3f, 0.8f);

    ////
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        b2World_Step(worldId, dt, 4);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Welcome to PinBall Game!", 190, 200, 20, LIGHTGRAY);
        DrawRectangleRec(table, DARKBLUE);
        DrawRectangleLinesEx(table, 5, RAYWHITE);

        b2Vec2 ballPos = b2Body_GetPosition(ballBody);
        Vector2 newBallPos = { m_to_px(ballPos.x), m_to_px(ballPos.y) };
        DrawCircleV(newBallPos, ball.radius, RED);


        b2Vec2 slant1Pos = b2Body_GetPosition(slantBody1);
        float slant1Angle = b2Rot_GetAngle(b2Body_GetRotation(slantBody1));
        DrawRectanglePro(
            (Rectangle){ m_to_px(slant1Pos.x), m_to_px(slant1Pos.y), Obstacle_W, Obstacle_H },
            (Vector2){ Obstacle_W/2, Obstacle_H/2 },
            slant1Angle * RAD2DEG,
            GRAY
        );

        b2Vec2 slant2Pos = b2Body_GetPosition(slantBody2);
        float slant2Angle = b2Rot_GetAngle(b2Body_GetRotation(slantBody2));
        DrawRectanglePro(
            (Rectangle){ m_to_px(slant2Pos.x), m_to_px(slant2Pos.y), Obstacle_W, Obstacle_H },
            (Vector2){ Obstacle_W/2, Obstacle_H/2 },
            slant2Angle * RAD2DEG,
            GRAY
        );

        Vector2 a1 = SlantCenterA(slant1);
        Vector2 b1 = SlantCenterB(slant1);
        Vector2 a2 = SlantCenterA(slant2);
        Vector2 b2 = SlantCenterB(slant2);
        DrawLineEx(a1, b1, 2.0f, RED);
        DrawLineEx(a2, b2, 2.0f, RED);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
