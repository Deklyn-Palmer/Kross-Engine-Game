/*
 *  Author: Chris Deitch.
 *  Editors:
 *		- Chris Deitch
 */
#include "../Core.h"

#include "Component.h"
namespace Kross
{
    class KROSS_API RopeProperties : public Component
    {
    private:
        std::vector<Vector2> positions;
        float chainLinkSize = 0.0f;
        bool isBreakable = false;
        bool isLinkedToStaticPoint = true;
        PhysicsScene* p_PhysicsScene;

    public:
        b2Filter filterBits;
        
        PhysicsScene* GetPhysicsScene() { return p_PhysicsScene; }
        std::vector<Vector2> GetPositions() { return positions; }
        float GetLinkSize() { return chainLinkSize; }
        bool GetStartIsStatic() { return isLinkedToStaticPoint; }
        bool GetIsBreakable() { return isBreakable; }
        
        void SetPhysicsScene(PhysicsScene* toSet) { p_PhysicsScene = toSet; }
        void SetPositions(std::vector<Vector2> positionsToSet);
        void SetLinkSize(float linkSize) {  chainLinkSize = linkSize; }
        void SetStartIsStatic(bool startIsStatic) { isLinkedToStaticPoint = startIsStatic; }
        void SetIsBreakable(bool breakable) { isBreakable = breakable; }






    };
}