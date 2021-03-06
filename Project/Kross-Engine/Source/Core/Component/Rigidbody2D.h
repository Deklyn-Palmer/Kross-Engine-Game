/*
 *  Author: Jake Warren.
 *  Editors:
 *      - Jake Warren.
 *      - Deklyn Palmer.
 *      - Chris Deitch
 */
#pragma once

#include "../Core.h"

#include "Renderer.h"
#include "../Physics/PhysicsScene.h"

#include "../Renderer/Shader/Shader.h"
#include "../Renderer/LineRenderer.h"
#include "../Physics/Physics.h"
#include "../Physics/Shape/Shape.h"
#include "../Renderer/Tilemap/Tile.h"
#include "../Renderer/Tilemap/TileSet.h"
#include "../Renderer/Tilemap/TileMap.h"

namespace Kross
{
    class KROSS_API Shape;
    class KROSS_API Object;

    class KROSS_API Rigidbody2D : public Renderer
    {
    private:
        Body* p_Body;
        PhysicsScene* p_PhysicsScene;
        Box* p_Box;
        Capsule* p_Capsule;
        Circle* p_Circle;
        b2Filter* p_Filter;
        b2RevoluteJoint* p_RevJoint;
        
        LineRenderer* p_DebugRenderer;

        ShapeType m_ShapeType;
        CollisionState m_CollisionState;
        RigidbodyState m_RigidbodyState;
        ColliderFilters m_ColliderFilter;

        RaycastData* p_RayData;
        AABBCollisionData* p_AABBCollisionData;

        std::vector<Body*> m_CloseObjects;
        std::vector<Body*> m_TileColliders;
        std::vector<Box*> m_TileShapes;
        std::vector<Circle*> m_TileCornerShapes;
        std::vector<FixtureDef*> m_Fixtures;
        std::vector<Body*> m_Bodies;
        std::vector<Vector2> m_CloseParticles;

        float m_Friction = 0.5f;
        b2Rot m_BottomWheelRotation;

        bool m_IsBullet = false;

    protected:
        friend class PhysicsScene;
        friend class Scene;
        friend class PlayerController;

        friend class Object;

        /* Rigidbody2D start method */
        void OnStart() override;

        /* Rigidbody2D update method */
        void OnUpdate() override;

        // Sends Draw Information to the Line Renderer.
        void OnUpdateDrawInformation();

        // Attaches a Line Renderer.
        void AttachLineRenderer(LineRenderer* renderer) { p_DebugRenderer = renderer; };

        Body* GetRayCollisionBody() const { return p_RayData->body; }

        /* Sets the physics scene */
        void SetPhysicsScene(PhysicsScene* scene) { p_PhysicsScene = scene; }

        /* Gets the physics scene */
        PhysicsScene* GetPhysicsScene() const { return p_PhysicsScene; }

    public:
        Rigidbody2D();
        ~Rigidbody2D();

        /* Gets the mass */
        float GetMass() { return p_Body->GetMass(); }

        /* Sets the Body for the RigidBody */
        void SetBody(Body* body) { p_Body = body; }

        /* Gets the Body for the RigidBody */
        Body* GetBody() const { return p_Body; }


        /* Apply a force to the body */
        void OnApplyForce(Vector2 force);

        /* Apply an impulse to the body */
        void OnApplyImpulse(Vector2 impulse);


        /* Creates a new dynamic circle */
        void CreateDynamicCircle(float radius, Vector2 pos, bool fixedRotation, b2Filter* filter, float friction = 0.5f, float density = 0.5f);

        /* Creates a new dynamic box */
        void CreateDynamicBox(Vector2 dimensions, Vector2 pos, bool fixedRotation, b2Filter* filter, float friction);

        void CreateDynamicCapsule(Vector2 dimensions, Vector2 pos, bool fixedRotation, b2Filter* filter, float friction = 0.5f);

        /* Creates a new static circle */
        void CreateWorldCircle(float radius, Vector2 pos, b2Filter* filter, float friction = 0.5f);

        /* Creates a new static box */
        void CreateWorldBox(Vector2 dimensions, Vector2 pos, b2Filter* filter, float friction = 0.5f);

        /* Gets the Objects Position */
        Vector2 GetPosition() const;

        /* Sets the collision state of the rigidbody */
        void SetCollisionState(CollisionState state) { m_CollisionState = state; }
        CollisionState GetCollisionState() { return m_CollisionState; }

        void SetRigidbodyState(RigidbodyState state) { m_RigidbodyState = state; }
        RigidbodyState GetRigidbodyState() { return (RigidbodyState)m_RigidbodyState; }

        /* Calculates the spring under the player */
        Vector2 SpringCalculation(Body* body1, Body* body2, float distance);

        void SetFriction(float friction) { m_Friction = friction; }
        float GetFriction() { return m_Friction; }

        /* Returns the current collision state */
        CollisionState GetCollisionState() const { return m_CollisionState; }

        /* Collides dynamic bodies with particles */
        Vector2 CollideParticles();

        /* Creates a ray cast in a specified direction */
        /* Returning the distance to an object if there is a hit */
        RaycastData* CalculateRayLength(float maxFraction, Vector2 direction, Vector2 pos);
        
        /* Checks a specified AABB for objects */
        std::vector<Body*> GetSurroundingObjects(float size, Body* body);

        /* Creates the colliders for the tile map */
        void CreateTileMapColliders(TileMap* tileMap, Tile* tile, float friction = 0.5f);

        /* Deletes the tile map colliders */
        void DeleteTileMapColliders();

        /* Checks for the players current state */
        void UpdateRigidbodyState();

        /* Adds a fixture to the list of fixtures */
        void AddFixtureDef(FixtureDef* fixture) { m_Fixtures.push_back(fixture); }

        /* Gets the Raycast data */
        RaycastData* GetRaycastData() { return p_RayData; }

        /* Updates the collisions */
        void CollisionUpdate();

        /* Sets the collider filter */
        void SetColliderFilter(b2Filter* filter) { p_Filter = filter; }
        /* Gets the collider filters */
        b2Filter* GetColliderFilters() { return p_Filter; }

        /* Takes a rev joint definition and creates a revolute joint */
        void CreateRevJoint(b2RevoluteJointDef revJointDef) { p_RevJoint = (b2RevoluteJoint*)p_PhysicsScene->GetPhysicsWorld()->CreateJoint(&revJointDef); }
        /* Gets the revolute joint */
        b2RevoluteJoint* GetRevJoint() { return p_RevJoint; }

        void AddCloseObject(Body* body) { m_CloseObjects.push_back(body); }
        void ClearCloseObjects() { m_CloseObjects.clear(); }
        std::vector<Body*> GetCloseObjects() { return m_CloseObjects; }

        void AddCloseParticle(Vector2 pos) { m_CloseParticles.push_back(pos); }
        void ClearCloseParticles() { m_CloseParticles.clear(); }
        std::vector<Vector2> GetCloseParticles() { return m_CloseParticles; }

        void SetBodyPos(Vector2 pos)
        {
            for (int i = 0; i < m_Bodies.size(); i++)
            {
                m_Bodies[i]->SetTransform(Getb2Vec2(pos), 0.0f);
            }
        }

        void SetBodyVelocity(Vector2 velocity)
        {
            for (int i = 0; i < m_Bodies.size(); i++)
            {
                m_Bodies[i]->SetLinearVelocity(Getb2Vec2(velocity));
            }
        }

        void SetContinuousCollision(bool isContinuous) { m_IsBullet = isContinuous; }

    };
}