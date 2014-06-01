#ifndef _B2HELPER_
#define _B2HELPER_

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "Object/B2CCNode.h"
USING_NS_CC;
using namespace std;

class B2Helper : b2ContactListener
{
public:
	static B2Helper* Instance();
	b2World* getWorld();
	void  setWorld(b2World *world);

	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

	void bodiesListener(float dt);

    void setMass(b2Body *body, int mass);


	void putSleepingPool(b2Body *body)
	{
		if(body != NULL)
			m_sleepingPool->push_back(body);
	}
	void putDeadPool(b2Body *body)
	{
		if(body	!= NULL)
			m_deadPool->push_back(body);
	}
    void putActivePool(b2Body *body)
    {
        if(body != NULL)
            m_activePool->push_back(body);
    }
    void putJointPool(b2JointDef* jointDef)
    {
        m_jointPool->push_back(jointDef);

        vector<b2JointDef*>::iterator iter;
        for(iter = m_jointPool->begin(); iter != m_jointPool->end(); ++iter)
        {

        }
    }

	inline void colliedLog(b2Contact *contact)
	{
		CCNode *nodeA, *nodeB;
		nodeA = (CCNode*)(contact->GetFixtureA()->GetBody()->GetUserData());
		nodeB = (CCNode*)(contact->GetFixtureB()->GetBody()->GetUserData());

		CCLog("tag %d begin collied with tag %d",nodeA == NULL ? 0 : nodeA->getTag(), nodeB == NULL? 0 : nodeB->getTag());
	}
protected:
private:
	inline void colliedDispatcher(b2Contact *contact);
	
	vector<b2Body*> *m_sleepingPool;
    vector<b2Body*> *m_activePool;
	vector<b2Body*> *m_deadPool;
    vector<b2JointDef*> *m_jointPool;
	B2Helper();
	b2World *m_world;
};

enum Box2dMask
{
    BM_EDGE = 0x0001,
    BM_ROLE = 0x0002,
    BM_BLOCK = 0x0004,
    BM_MONSTOR = 0x0008,
    BM_WEAPON = 0x0010
};
class ICollideable
{
public :
	virtual void onCollied(b2Contact* contact, b2Body *bodyOther) {};
};
#endif