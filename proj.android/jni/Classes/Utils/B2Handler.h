#include "Box2D/Box2D.h"
#include "cocos2d.h"
#include "Object/B2CCNode.h"
#include "Utils/TagHelper.h"
#include "Object/RoleObject.h"
#include "Object/Block.h"
#include "Object/Monster.h"

USING_NS_CC;

class B2Handler : b2ContactListener
{
public:
    ~B2Handler();
    static B2Handler* createB2Handler(b2World *world);

    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
    void BeginContact(b2Contact* contact);
    void EndContact(b2Contact* contact);

    void colliedDispatcher(b2Contact *contact);

    inline void colliedLog(b2Contact *contact)
    {
        CCNode *nodeA, *nodeB;
        nodeA = (CCNode*)(contact->GetFixtureA()->GetBody()->GetUserData());
        nodeB = (CCNode*)(contact->GetFixtureB()->GetBody()->GetUserData());

        CCLog("tag %d begin collied with tag %d",nodeA == NULL ? 0 : nodeA->getTag(), nodeB == NULL? 0 : nodeB->getTag());
    }
protected:
    B2Handler();
    B2Handler(b2World* world);

private:
};
