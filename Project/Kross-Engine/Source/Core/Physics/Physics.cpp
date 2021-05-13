/*
 *  Author: Jake Warren.
 *  Editors:
 *      - Jake Warren.
 *      - Deklyn Palmer.
 */

#include "Physics.h"


namespace Kross
{
    Body* Physics::p_Body = nullptr;
    Physics* Physics::p_Instance = nullptr;
    Vector2 Physics::m_IntersectionNormal = Vector2(0,0);
    LineRenderer* Physics::p_Lines = nullptr;
    Raycast2DCallback* Physics::p_RayCallback = new Raycast2DCallback();
    CollisionData* Physics::p_CollisionData = new CollisionData();
    RaycastData* Physics::p_RayData = nullptr;

    void Physics::OnCreate()
    {
        if (!p_Instance)
            p_Instance = new Physics();
    }

    void Physics::OnDestroy()
    {
        if (p_Instance)
            delete p_Instance;
    }

    RaycastData* Physics::OnRayCast(Vector2 pos, Vector2 direction, Body* body, float max, LineRenderer* lines)
    {
        body->GetWorld()->RayCast(p_RayCallback, { pos.x, pos.y }, { pos.x + direction.x * max, pos.y + direction.y * max });

        p_RayData = p_RayCallback->GetRayData();

        return p_RayData;
    }
}