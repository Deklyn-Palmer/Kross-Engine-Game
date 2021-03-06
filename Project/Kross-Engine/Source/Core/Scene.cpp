/*
 *  Author: Deklyn Palmer.
 *  Editors:
 *      - Deklyn Palmer.
 *      - Jake Warren.
 */

#include "Scene.h"

#include "Component/Camera.h"
#include "Manager/ShaderManager.h"
#include "Manager/SceneManager.h"
#include "Manager/Time.h"

#include "Editor/Editor.h"

#include "Input.h"

#include "Application.h"

namespace Kross
{
    Scene::~Scene()
    {
        /* Clean up Dynamic Objects. */
        for (int i = 0; i < m_Objects.size(); i++)
        {
            Object::OnDestroy(m_Objects[i]);
            m_Objects[i] = nullptr;
        }

        /* Clean up Static Objects. */
        for (int i = 0; i < m_StaticObjects.size(); i++)
        {
            Object::OnDestroy(m_StaticObjects[i]);
            m_StaticObjects[i] = nullptr;
        }

        /* Clear the Render Queue. */
        m_RenderList.clear();

        for (int i = 0; i < m_BatchRenderers.size(); i++)
        {
            BatchRenderer::OnDestroy(m_BatchRenderers[i]);
            m_BatchRenderers[i] = nullptr;
        }

        m_BatchRenderers.clear();

        #ifdef KROSS_EDITOR
        Object::OnDestroy(m_EditorCamera);
        Object::OnDestroy(m_EditorObjectTool);
        #endif

        delete m_Physics;
        delete m_WorldFilter;
        delete m_DebugRenderer;

        m_DebugShader = nullptr;
    }

    void Scene::OnStart()
    {
        if (m_Started) return;

        for (int i = 0; i < m_BatchRenderers.size(); i++)
        {
            m_BatchRenderers[i]->OnStart();
        }

        /* Grab the Debug Shader. */
        m_DebugShader = ResourceManager::GetResource<Shader>("LineShader");
        m_DebugRenderer->Initialise();

        /* Start All Objects. */
        for (int i = 0; i < m_Objects.size(); i++)
        {
            m_Objects[i]->OnStart();
        }

        for (int i = 0; i < m_StaticObjects.size(); i++)
        {
            m_StaticObjects[i]->OnStart();
        }

        #ifdef KROSS_EDITOR 
        // Start the Editor Camera
        m_EditorCamera->OnStart();
        m_EditorObjectTool->OnStart();
        #endif

        /* Scene has Started. */
        m_Started = true;
    }

    void Scene::OnUpdate()
    {
        #ifdef KROSS_EDITOR
        Editor::MoveEditorCamera(m_EditorCamera);
        m_EditorCamera->OnUpdate();
        m_EditorObjectTool->OnUpdate();
        #endif

        /* Update all Dynamic Objects. */
        for (int i = 0; i < m_Objects.size(); i++)
        {
            m_Objects[i]->OnUpdate();
        }
    }

    void Scene::OnPhysicsUpdate()
    {
        int refreshRate = Application::GetWindow()->GetScreenRefreshRate();
        int particleIterations = static_cast<int>(glm::ceil(2 * (60.0f / static_cast<float>(refreshRate))));
        int positionIterations = static_cast<int>(glm::ceil(8 * (60.0f / static_cast<float>(refreshRate))));
        int velocityIterations = static_cast<int>(glm::ceil(16 * (60.0f / static_cast<float>(refreshRate))));

        /* Update the physics step */
        if (Application::GetWindow()->GetVSync())
        {
            /* TO DO */
            m_Physics->GetPhysicsWorld()->Step(1.0f / (float)refreshRate, velocityIterations, positionIterations, particleIterations);
        }
        else
        {
            m_Physics->GetPhysicsWorld()->Step(Time::GetDeltaTime(), 8, 3, 2); /* Not recommended. */
        }
    }

    void Scene::OnRender()
    {
        /* Get the Camera Component. if we have a Camera. */
        if (m_Camera)
        {
            Camera* camera = m_Camera->GetComponent<Camera>();
            ShaderManager::OnUpdateShaderVPMatrix(camera->GetView(), camera->GetProjection());
        }
        
        /* Render all Game Objects. */
        
        /* For removing the Objects from the Render Queue that are dynamic. */
        std::vector<int> dynamicRenderQueueReferencePoints;
        
        /* Go through the Dynamic Object List. To place them in the Render Queue. */
        for (int i = 0; i < m_Objects.size(); i++)
        {
            /* If the Obejct is Enabled. Add it.*/
            if (m_Objects[i]->Enabled() && m_Objects[i]->GetRenderableStatus())
            {
                /* Keep Record of the index, so we know where in the list it is to remove it. */
                std::vector<int> indexes = AttachObjectToRenderQueue(m_Objects[i]);
        
                for (int j = 0; j < indexes.size(); j++)
                {
                    dynamicRenderQueueReferencePoints.push_back(indexes[j]);
                }
            }
        }
        
        /* Render the Objects. */
        for (int l = 0; l < (int)Layer::Count; l++)
        {   
            for (int i = 0; i < m_RenderList[l].size(); i++)
            {
                m_BatchRenderers[l]->AttachRenderer(m_Camera->GetComponent<Camera>(), m_RenderList[l][i]);
            }
        
            m_BatchRenderers[l]->OnFinish();
        }

        #ifdef KROSS_DEBUG
        /* Draw Debug Information. */
        m_DebugShader->Bind();
        m_DebugRenderer->UpdateFrame();

        #else
        m_DebugRenderer->Clear();
        #endif



        /* Remove the Dynamic Objects from the Render Queue. */
        for (int i = 0; i < m_Objects.size(); i++)
        {
            DetachObjectFromRenderQueue(m_Objects[i]->GetLayer(), m_Objects[i]);
        }
    }

    void Scene::OnRemoveObjects()
    {
        /* Go through the Dynamic Objects List. */
        for (int i = 0; i < m_Objects.size(); i++)
        {
            /* If the Object that needs to be deleted is found. */
            if (m_Objects[i]->ShouldBeRemoved())
            {
                DetachObjectFromRenderQueue(m_Objects[i]->GetLayer(), m_Objects[i]);

                /* Null it out on the dynamic list and erase. */
                m_Objects[i] = nullptr;
                m_Objects.erase(m_Objects.begin() + i);
            }
        }
        /* Go through the Static Objects List. */
        for (int i = 0; i < m_StaticObjects.size(); i++)
        {
            /* If the Object that needs to be deleted is found. */
            if (m_StaticObjects[i]->ShouldBeRemoved())
            {
                DetachObjectFromRenderQueue(m_StaticObjects[i]->GetLayer(), m_StaticObjects[i]);

                /* Null it out on the static list and erase. */
                m_StaticObjects[i] = nullptr;
                m_StaticObjects.erase(m_StaticObjects.begin() + i);
            }
        }

        for (int i = 0; i < m_ActualObjects.size(); i++)
        {
            if (m_ActualObjects[i]->ShouldBeRemoved())
            {
                /* Destroy the Object. */
                Object::OnDestroy(m_ActualObjects[i]);

                /* Erase it from the Global Objects list. */
                m_ActualObjects[i] = nullptr;
                m_ActualObjects.erase(m_ActualObjects.begin() + i);
            }
        }
    }

    void Scene::OnUpdateCameraAspectRatio(float aspectRatio)
    {
        /* If we have a Camera. */
        if (m_Camera)
        {
            m_Camera->GetComponent<Camera>()->SetAspectRatio(aspectRatio);
        }
    }

    std::vector<int> Scene::AttachObjectToRenderQueue(Object* object)
    {
        /* List of all the indexes Recorded on this Object. */
        std::vector<int> indexes = std::vector<int>();

        /* If there is no renderer on the object. */
        if (!object->GetRenderableStatus())
        {
            return indexes; /* Return Empty. */
        }
        /* This is to return where abouts the Object sits in the Render Queue. */
        int index = 0;

        /* The Layer ID. */
        int layer = (int)object->GetLayer();

        /* Renderer Component List. */
        std::vector<Renderer*> objectRenderList = object->GetRendererComponents();

        /* Go through the Object Render List. */
        for (int j = 0; j < objectRenderList.size(); j++)
        {
            /* If the Scenes's layer list is larger than zero. */
            if (m_RenderList[layer].size() > 0)
            {
                /* Run through the Layer List. */
                for (int i = 0; i < m_RenderList[layer].size(); i++)
                {
                    /* Check if the Depth of the object is larger than the one currently being checked in the list. */
                    if (objectRenderList[j]->GetDepth() > m_RenderList[layer][i]->GetDepth())
                    {
                        index++; /* Keep going down the list. */
                    }
                    /* We have found a spot in the Render Queue. */
                    else
                    {
                        break; /* Stop the Search. */
                    }
                }

                /* Add the Object to the Render Queue. */
                m_RenderList[layer].emplace(m_RenderList[layer].begin() + index, objectRenderList[j]);

                /* Add the Indexes as well. */
                indexes.push_back(index);

                /* Reset Index. */
                index = 0;
            }

            /* If not... */
            else
            {
                /* Attach as the First one. */
                m_RenderList[layer].push_back(objectRenderList[j]);

                /* Add the Indexes as well. */
                indexes.push_back(index);

                /* Reset Index. */
                index = 0;
            }
        }

        /* Return the Index. */
        return indexes;
    }

    void Scene::SetCamera(Object* object)
    {
        /* Check if the Object is of type Camera. */
        Camera* camera = object->GetComponent<Camera>();

        /* if the Object is a Camera. */
        if (camera)
        {
            /* If we have no Camera, set it. */
            if (!m_Camera)
            {
                m_Camera = object;
            }
        }

        return;
    }

    void Scene::DetachObjectFromRenderQueue(Layer layer, Object* object)
    {
        /* Early out if there is no renderer on the object. */
        if (!object->GetRenderableStatus())
        {
            return;
        }

        std::vector<Renderer*> objectRenderList = object->GetRendererComponents();

        for (int j = 0; j < objectRenderList.size(); j++)
        {
            /* Go through Layer Objects. */
            for (int i = 0; i < m_RenderList[(int)layer].size(); i++)
            {
                /* If this is the Component we are looking for. */
                if (m_RenderList[(int)layer][i] == objectRenderList[j])
                {
                    DetachObjectFromRenderQueue(layer, i); /* Remove it. */
                }
            }
        }
    }

    void Scene::DetachObjectFromRenderQueue(Layer layer, int index)
    {
        /* Remove the Object from the List. */
        m_RenderList[(int)layer][index] = nullptr;
        m_RenderList[(int)layer].erase(m_RenderList[(int)layer].begin() + index);
    }

    Scene* Scene::OnCreate(const std::string& name)
    {
        /* Create the Scene. */
        Scene* scene = KROSS_NEW Scene(name);

        /* Return the new Scene. */
        return scene;
    }

    void Scene::OnDestroy(Scene* scene)
    {
        /* Safe programming, not really needed but good to have. */
        if (scene)
            delete scene;
    }

    void Scene::AttachObject(Object* object, bool startOverride)
    {
        /* Attach the Object to the global object list. */
        m_ActualObjects.push_back(object);

        /* Checks if the Object has a Camera. */
        SetCamera(object);

        object->SetPhysicsScene(m_Physics);
        object->SetLineRenderer(m_DebugRenderer);

        /* Check if the object is type RigidBody2D */
        Rigidbody2D* body = object->GetComponent<Rigidbody2D>();

        /* If the object is a RigidBody the physics scene is set */
        if (body)
        {
            body->SetPhysicsScene(m_Physics);
            body->AttachLineRenderer(m_DebugRenderer);
        }

        Cog* cog = object->GetComponent<Cog>();
        if (cog) 
        {
            cog->SetPhysicsScene(m_Physics);
            
            
        }



        /* Check if the object is type Particle Emitter */
        ParticleEmitter* emitter = object->GetComponent<ParticleEmitter>();

        /* If the object is a ParticleEmitter the physics scene is set */
        if (emitter)
            emitter->SetPhysicsScene(m_Physics);

        RopeAvatar* avatar = object->GetComponent<RopeAvatar>();

        if (avatar)
        {
            avatar->SetPhysicsScene(m_Physics);
            avatar->SetDebugRenderer(m_DebugRenderer);
        }

        /* If the Object is Static. */
        if (object->IsStatic())
        {
            /* Attach the Object to the Static list and place it in the render Queue. */
            AttachObjectToRenderQueue(object);
            m_StaticObjects.push_back(object);
        }
        /* If it's not. */
        else
        {
            m_Objects.push_back(object); /* Attach the Object to the Dynamic List. */
        }

        /* Start the object inside of the Scene. */
        if (m_Started || startOverride)
        {
            object->OnStart();
        }
    }

    void Scene::DetachObject(const std::string& name)
    {
        /* Find the Object and remove it from the list. */
        for (int i = 0; i < m_ActualObjects.size(); i++)
        {
            if (m_ActualObjects[i]->GetName() == name)
            {
                DetachObject(i); /* If the Object has been found, remove it from the list. */
            }
        }
    }

    void Scene::DetachObject(Object* object)
    {
        /* Find the Object and remove it from the list. */
        for (int i = 0; i < m_ActualObjects.size(); i++)
        {
            if (m_ActualObjects[i] == object)
            {
                DetachObject(i); /* If the Object has been found, remove it from the list. */
            }
        }
    }

    void Scene::DetachObject(int index)
    {
        if (index < 0 && index >= m_ActualObjects.size())
            return;


        m_ActualObjects[index]->m_Remove = true;
    }

    Object* Scene::FindObject(std::string name)
    {
        for (int i = 0; i < m_ActualObjects.size(); i++)
        {
            if (m_ActualObjects[i]->GetName() == name)
            {
                return m_ActualObjects[i];
            }
        }

        return nullptr;
    }
}