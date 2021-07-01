/*
 *  Author: Jake Warren.
 *  Editors:
 *      - Jake Warren.
 *      - Deklyn Palmer.
 */

#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "Rigidbody2D.h"
#include "../Physics/Shape/Shape.h"
#include "../Manager/Time.h"

namespace Kross
{
    Rigidbody2D::Rigidbody2D() :
        p_Body              (nullptr),
        p_PhysicsScene      (nullptr),
        p_Box               (nullptr),
        p_Circle            (nullptr),
        m_ShapeType         (ShapeType::Count),
        m_CollisionState    (CollisionState::None),
        p_FixtureDef        (KROSS_NEW FixtureDef()),
        p_MassData          (KROSS_NEW b2MassData()),
        p_RayData           (KROSS_NEW RaycastData()),
        p_Filter            (KROSS_NEW ContactFilter()),
        p_FluidData         (KROSS_NEW FluidCollisionData())
    {}

    Rigidbody2D::~Rigidbody2D()
    {
        p_Body = nullptr;
        p_PhysicsScene = nullptr;

        delete p_Circle;
        delete p_Box;

        delete p_MassData;
        delete p_RayData;
        delete p_FixtureDef;

        delete p_Filter;
        delete p_FluidData;
    }

    void Rigidbody2D::CreateDynamicCircle(float radius, Vector2 pos, bool fixedRotation, uint16 categoryBits, uint16 maskBits)
    {
        /* Sets the shape type */
        m_ShapeType = ShapeType::Circle;

        /* Create a bodyDef and set the variables */
        BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(pos.x, pos.y);

        /* Creates the body and assigns it to the pointer */
        p_Body = p_PhysicsScene->GetPhysicsWorld()->CreateBody(&bodyDef);
        p_Body->SetUserData(GetLinkObject());

        p_Body->SetFixedRotation(fixedRotation);

        /* Creates the cirlce */
        CircleShape circleShape;
        /* Sets the circles radius */
        circleShape.m_radius = radius;

        /* Creates a fixtureDef and assigns the variables */
        p_FixtureDef->shape = &circleShape;
        p_FixtureDef->density = 0.5f;
        p_FixtureDef->friction = 0.5f;
        p_FixtureDef->filter.categoryBits = categoryBits;
        p_FixtureDef->filter.maskBits = maskBits;

        p_Body->CreateFixture(p_FixtureDef);

        p_PhysicsScene->AttachBody(p_Body);

        /* Assigns the shape to the pointer */
        p_Circle = KROSS_NEW Circle(radius, Vector2(0, 0));
    }

    void Rigidbody2D::CreateDynamicCircle(float radius, float friction, Vector2 pos, bool fixedRotation, uint16 categoryBits, uint16 maskBits)
    {
        /* Sets the shape type */
        m_ShapeType = ShapeType::Circle;

        /* Create a bodyDef and set the variables */
        BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(pos.x, pos.y);

        /* Creates the body and assigns it to the pointer */
        p_Body = p_PhysicsScene->GetPhysicsWorld()->CreateBody(&bodyDef);
        p_Body->SetUserData(GetLinkObject());

        p_Body->SetFixedRotation(fixedRotation);

        /* Creates the cirlce */
        CircleShape circleShape;
        /* Sets the circles radius */
        circleShape.m_radius = radius;

        /* Creates a fixtureDef and assigns the variables */
        p_FixtureDef->shape = &circleShape;
        p_FixtureDef->density = 0.5f;
        p_FixtureDef->friction = friction;
        p_FixtureDef->filter.categoryBits = categoryBits;
        p_FixtureDef->filter.maskBits = maskBits;

        p_Body->CreateFixture(p_FixtureDef);

        p_PhysicsScene->AttachBody(p_Body);

        /* Assigns the shape to the pointer */
        p_Circle = KROSS_NEW Circle(radius, Vector2(0, 0));
    }

    void Rigidbody2D::CreateDynamicBox(Vector2 Dimensions, Vector2 pos, bool fixedRotation, uint16 categoryBits, uint16 maskBits)
    {
        /* Sets the shape type */
        m_ShapeType = ShapeType::Box;

        /* Create a bodyDef and set the variables */
        BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(pos.x, pos.y);
        //bodyDef.linearDamping = 2.0f;

        /* Creates the body and assigns it to the pointer */
        p_Body = p_PhysicsScene->GetPhysicsWorld()->CreateBody(&bodyDef);
        p_Body->SetUserData(GetLinkObject());

        /* Creates the shape */
        PolygonShape dynamicBox;
        /* Sets the shape as a box */
        dynamicBox.SetAsBox(Dimensions.x / 2.0f, Dimensions.y / 2.0f);

        /* Creates a fixtureDef and assigns all variables */
        p_FixtureDef->shape = &dynamicBox;
        p_FixtureDef->density = 1.0f;
        p_FixtureDef->friction = 0.5f;
        p_FixtureDef->filter.categoryBits = categoryBits;
        p_FixtureDef->filter.maskBits = maskBits;

        p_Body->CreateFixture(p_FixtureDef);
        p_Body->SetFixedRotation(fixedRotation);

        p_PhysicsScene->AttachBody(p_Body);

        /* Assigns the shape to the pointer */
        p_Box = KROSS_NEW Box(Dimensions, Vector2(0, 0));
    }

    void Rigidbody2D::CreateDynamicBox(Vector2 Dimensions, float friction, Vector2 pos, bool fixedRotation, uint16 categoryBits, uint16 maskBits)
    {
        /* Sets the shape type */
        m_ShapeType = ShapeType::Box;

        /* Create a bodyDef and set the variables */
        BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(pos.x, pos.y);
        //bodyDef.linearDamping = 2.0f;

        /* Creates the body and assigns it to the pointer */
        p_Body = p_PhysicsScene->GetPhysicsWorld()->CreateBody(&bodyDef);
        p_Body->SetUserData(GetLinkObject());

        /* Creates the shape */
        PolygonShape dynamicBox;
        /* Sets the shape as a box */
        dynamicBox.SetAsBox(Dimensions.x / 2.0f, Dimensions.y / 2.0f);

        /* Creates a fixtureDef and assigns all variables */
        p_FixtureDef->shape = &dynamicBox;
        p_FixtureDef->density = 1.0f;
        p_FixtureDef->friction = friction;
        p_FixtureDef->filter.categoryBits = categoryBits;
        p_FixtureDef->filter.maskBits = maskBits;

        p_Body->CreateFixture(p_FixtureDef);
        p_Body->SetFixedRotation(fixedRotation);

        p_PhysicsScene->AttachBody(p_Body);

        /* Assigns the shape to the pointer */
        p_Box = KROSS_NEW Box(Dimensions, Vector2(0, 0));
    }

    void Rigidbody2D::CreateWorldCircle(float radius, Vector2 pos, uint16 categoryBits, uint16 maskBits)
    {
        /* Sets the shape type */
        m_ShapeType = ShapeType::Circle;

        /* Create a bodyDef and set the variables */
        BodyDef bodyDef;
        bodyDef.type = b2_staticBody;
        bodyDef.position.Set(pos.x, pos.y);

        /* Creates the body and assigns it to the pointer */
        p_Body = p_PhysicsScene->GetPhysicsWorld()->CreateBody(&bodyDef);
        p_Body->SetUserData(GetLinkObject());

        /* Creates the cirlce */
        CircleShape circleShape;
        /* Sets the circles radius */
        circleShape.m_radius = radius;

        /* Creates a fixtureDef and assigns the variables */
        p_FixtureDef->shape = &circleShape;
        p_FixtureDef->density = 0.5f;
        p_FixtureDef->friction = 0.5f;
        p_FixtureDef->filter.categoryBits = categoryBits;
        p_FixtureDef->filter.maskBits = maskBits;

        p_Body->CreateFixture(p_FixtureDef);

        p_PhysicsScene->AttachBody(p_Body);

        /* Assigns the shape to the pointer */
        p_Circle = KROSS_NEW Circle(radius, Vector2(0, 0));
    }

    void Rigidbody2D::CreateWorldCircle(float radius, float friction, Vector2 pos, uint16 categoryBits, uint16 maskBits)
    {
        /* Sets the shape type */
        m_ShapeType = ShapeType::Circle;

        /* Create a bodyDef and set the variables */
        BodyDef bodyDef;
        bodyDef.type = b2_staticBody;
        bodyDef.position.Set(pos.x, pos.y);

        /* Creates the body and assigns it to the pointer */
        p_Body = p_PhysicsScene->GetPhysicsWorld()->CreateBody(&bodyDef);
        p_Body->SetUserData(GetLinkObject());

        /* Creates the cirlce */
        CircleShape circleShape;
        /* Sets the circles radius */
        circleShape.m_radius = radius;

        /* Creates a fixtureDef and assigns the variables */
        p_FixtureDef->shape = &circleShape;
        p_FixtureDef->density = 0.5f;
        p_FixtureDef->friction = friction;
        p_FixtureDef->filter.categoryBits = categoryBits;
        p_FixtureDef->filter.maskBits = maskBits;

        p_Body->CreateFixture(p_FixtureDef);

        p_PhysicsScene->AttachBody(p_Body);

        /* Assigns the shape to the pointer */
        p_Circle = KROSS_NEW Circle(radius, Vector2(0, 0));
    }

    void Rigidbody2D::CreateWorldBox(Vector2 Dimensions, Vector2 pos, uint16 categoryBits, uint16 maskBits)
    {
        /* Sets the shape type */
        m_ShapeType = ShapeType::Box;

        /* Create a bodyDef and set the variables */
        BodyDef bodyDef;
        bodyDef.type = b2_staticBody;
        bodyDef.position.Set(pos.x, pos.y);

        /* Creates the body and assigns it to the pointer */
        p_Body = p_PhysicsScene->GetPhysicsWorld()->CreateBody(&bodyDef);
        p_Body->SetUserData(GetLinkObject());

        /* Creates the shape */
        PolygonShape dynamicBox;
        /* Sets the shape as a box */
        dynamicBox.SetAsBox(Dimensions.x / 2.0f, Dimensions.y / 2.0f);

        /* Creates a fixtureDef and assigns all variables */
        p_FixtureDef->shape = &dynamicBox;
        p_FixtureDef->density = 1.0f;
        p_FixtureDef->friction = 0.5f;
        p_FixtureDef->filter.categoryBits = categoryBits;
        p_FixtureDef->filter.maskBits = maskBits;

        p_Body->CreateFixture(p_FixtureDef);

        p_PhysicsScene->AttachBody(p_Body);

        /* Assigns the shape to the pointer */
        p_Box = KROSS_NEW Box(Dimensions, Vector2(0, 0));
    }

    void Rigidbody2D::CreateWorldBox(Vector2 Dimensions, float friction, Vector2 pos, uint16 categoryBits, uint16 maskBits)
    {
        /* Sets the shape type */
        m_ShapeType = ShapeType::Box;

        /* Create a bodyDef and set the variables */
        BodyDef bodyDef;
        bodyDef.type = b2_staticBody;
        bodyDef.position.Set(pos.x, pos.y);

        /* Creates the body and assigns it to the pointer */
        p_Body = p_PhysicsScene->GetPhysicsWorld()->CreateBody(&bodyDef);
        p_Body->SetUserData(GetLinkObject());
        /* Creates the shape */
        PolygonShape dynamicBox;
        /* Sets the shape as a box */
        dynamicBox.SetAsBox(Dimensions.x / 2.0f, Dimensions.y / 2.0f);

        /* Creates a fixtureDef and assigns all variables */
        p_FixtureDef->shape = &dynamicBox;
        p_FixtureDef->density = 1.0f;
        p_FixtureDef->friction = friction;
        p_FixtureDef->filter.categoryBits = categoryBits;
        p_FixtureDef->filter.maskBits = maskBits;

        p_Body->CreateFixture(p_FixtureDef);

        p_PhysicsScene->AttachBody(p_Body);

        /* Assigns the shape to the pointer */
        p_Box = KROSS_NEW Box(Dimensions, Vector2(0, 0));
    }

    void Rigidbody2D::OnStart()
    {
        /* Gets the body */
        p_Body = GetBody();

        /* Gets the physics scene */
        p_PhysicsScene = GetPhysicsScene();

        /* Send Draw Information. */
        p_DebugRenderer->DrawRigidBody(p_Body);
    }

    void Rigidbody2D::OnUpdate()
    {
        if (p_Box != nullptr)
        {
            /* Checks if the object is not static */
            if (p_Body->GetType() != b2_staticBody)
            {
                Vector2 particleForce = CollideParticles();

                if (p_FluidData->m_Collision)
                {
                    // player is swimming
                    OnApplyForce(particleForce * (p_Body->GetMass() * 0.5f));
                }

                float footSpringLength = 0.5f;

                p_RayData->maxFraction = 1.0f;
                p_RayData->direction = Vector2(0, -footSpringLength);
                p_RayData->pos = Vector2(p_Body->GetPosition().x, p_Body->GetPosition().y);

                p_RayData = Physics::OnRayCast(p_RayData->pos, p_RayData->direction, p_Body, p_RayData->maxFraction);

                float distance = glm::length(p_RayData->intersectionPoint - Vector2(p_Body->GetPosition().x, p_Body->GetPosition().y));

                if (p_RayData->hit)
                {
                    if (m_CollisionState == CollisionState::None)
                        m_CollisionState = CollisionState::Enter;

                    else if (m_CollisionState == CollisionState::Enter)
                        m_CollisionState = CollisionState::Stay;

                    //OnApplyForce(SpringCalculation(p_Body, p_RayData->body, distance) * p_RayData->intersectionNormal);

                    p_RayData->hit = false;
                }
                else
                {
                    p_RayData->intersectionPoint = Vector2(p_Body->GetPosition().x, p_Body->GetPosition().y - footSpringLength);

                    if (m_CollisionState == CollisionState::Enter || m_CollisionState == CollisionState::Stay)
                        m_CollisionState = CollisionState::Exit;

                    else if (m_CollisionState == CollisionState::Exit)
                        m_CollisionState = CollisionState::None;
                }
                
                /* Gets the object position and updates it with the position of the body */
                GetLinkObject()->GetTransform()->m_Position = Vector2(p_Body->GetPosition().x, p_Body->GetPosition().y);

                /* Gets the object rotation and updates it with the angle of the body */
                GetLinkObject()->GetTransform()->m_Rotation = glm::degrees(p_Body->GetAngle());
            }
        }
        else if (p_Circle != nullptr)
        {
            /* Checks if the object is not static */
            if (p_Body->GetType() != b2_staticBody)
            {
                Vector2 particleForce = CollideParticles();

                if (p_FluidData->m_Collision)
                {
                    // player is swimming
                    OnApplyForce(particleForce * (p_Body->GetMass() * 0.5f));
                }

                /* Gets the object position and updates it with the position of the body */
                GetLinkObject()->GetTransform()->m_Position = Vector2(p_Body->GetPosition().x, p_Body->GetPosition().y);
            
                /* Gets the object rotation and updates it with the angle of the body */
                GetLinkObject()->GetTransform()->m_Rotation = glm::degrees(p_Body->GetAngle());
            }
        }
    }

    void Rigidbody2D::OnApplyForce(Vector2 force)
    {
        if (glm::length(force) > 0.0001f)
        {
            p_Body->ApplyForceToCenter({ force.x, force.y }, true);
        }
    }

    void Rigidbody2D::OnApplyImpulse(Vector2 impulse)
    {
        p_Body->ApplyLinearImpulse({ impulse.x, impulse.y }, { p_Body->GetPosition().x, p_Body->GetPosition().y }, true);
    }

    Vector2 Rigidbody2D::GetPosition() const
    {
        return Vector2(GetLinkObject()->GetTransform()->m_Position.x, GetLinkObject()->GetTransform()->m_Position.y);
    }
    
    Vector2 Rigidbody2D::SpringCalculation(Body* body1, Body* body2, float dist)
    {
        Vector2 fs = { 0,0 };
        Vector2 velocity = { 0,0 };
        Vector2 acceleration = { 0,0 };
        float mass = 0.0f;

        Vector2 pos1 = Vector2(body1->GetPosition().x, body1->GetPosition().y);
        Vector2 pos2 = p_RayData->intersectionPoint;
        float distance = glm::length(pos2 - pos1);

        float distanceScale = 10.0f;
        float springConstant = 1.75f;
        float dampingConstant = 11.0f;
        float restLength = 0.55f;
        float size = 0.35f;

        float scalar = distanceScale * springConstant * (size - restLength);
        Vector2 direction = p_RayData->intersectionNormal;
        
        float s1 = glm::dot(Vector2(body1->GetLinearVelocity().x, body1->GetLinearVelocity().y), direction);
        float s2 = glm::dot(Vector2(body2->GetLinearVelocity().x, body2->GetLinearVelocity().y), direction);
        
        float dampingScalar = -dampingConstant * (s1 + s2);
        
        if (body1->GetMass() == 0)
        {
            fs = (scalar + dampingScalar) * direction;
            mass = body2->GetMass();
        }
        else
        {
            fs = (-scalar + dampingScalar) * direction;
            mass = body1->GetMass();
        }
        
        Vector2 fg = Vector2(Vector2(body1->GetWorld()->GetGravity().x, body1->GetWorld()->GetGravity().y) * mass);
        Vector2 force = fs + fg;
        

        return (springConstant * (size - distance)) * force;

    }

    Vector2 Rigidbody2D::CollideParticles()
    {
        Vector2 totalVelocity(0, 0);
        Vector2 averageVelocity(0, 0);
        int particleCount = 0;

        Physics::GetFluidCollisionCallBack()->SetFluidCollisionData(p_FluidData);
        const b2Shape* shape = p_Body->GetFixtureList()->GetShape();

        p_PhysicsScene->GetParticleSystem()->QueryShapeAABB(Physics::GetFluidCollisionCallBack(), *(b2Shape*)shape, p_Body->GetTransform());
        p_FluidData = Physics::GetFluidCollisionCallBack()->GetFluidCollisionData();

        particleCount = p_FluidData->m_ParticleIndexs.size();

        if (particleCount != 0)
        {
            p_FluidData->m_Collision = true;

            averageVelocity = Vector2(0, 0);
            totalVelocity = Vector2(0, 0);

            for (int i = 0; i < p_FluidData->m_ParticleIndexs.size(); i++)
            {
                Vector2 particleVelocity = Vector2(p_FluidData->p_ParticleSystem->GetVelocityBuffer()[p_FluidData->m_ParticleIndexs[i]].x, p_FluidData->p_ParticleSystem->GetVelocityBuffer()[p_FluidData->m_ParticleIndexs[i]].y);

                totalVelocity += particleVelocity;
            }

            p_FluidData->m_ParticleIndexs.clear();
            averageVelocity = glm::normalize(Vector2(totalVelocity.x / particleCount, totalVelocity.y / particleCount));
            particleCount = 0;
        }
        else
        {
            p_FluidData->m_Collision = false;
            averageVelocity = Vector2(0, 0);
        }

        return averageVelocity;
    }

    float Rigidbody2D::GetFriction()
    {
        return p_Body->GetFixtureList()->GetFriction();
    }

}