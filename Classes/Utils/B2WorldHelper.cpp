#include "Utils/B2WorldHelper.h"
#include "Object/RoleObject.h"
#include "Object/Block.h"
B2Helper::B2Helper()
{
	m_deadPool = new vector<b2Body*>();
	m_sleepingPool = new vector<b2Body*>();
    m_activePool = new vector<b2Body*>();
    m_jointPool = new vector<b2JointDef*>();
}

b2World* B2Helper::getWorld()
{
	return m_world;
}

 void B2Helper::setWorld(b2World *world)
{
	m_world = world;

	//m_world->SetContactListener(this);
}

B2Helper* B2Helper::Instance()
{
	static B2Helper *instance= new B2Helper();

	return instance;
}

void B2Helper::BeginContact(b2Contact* contact)
{
    colliedLog(contact);


}

void B2Helper::EndContact(b2Contact* contact)
{

}

void B2Helper::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
    colliedLog(contact);
    colliedDispatcher(contact);
}

void B2Helper::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{

}

void B2Helper::colliedDispatcher(b2Contact *contact)
{

    b2Body *bodyA, *bodyB;
    bodyA = contact->GetFixtureA()->GetBody();
    bodyB = contact->GetFixtureB()->GetBody();

    B2CCNode *node = (B2CCNode*)(bodyA->GetUserData());
    if(node != NULL&& TagHelper::Instance()->isObject(node->getTag(), ON_ROLE))
    {
        node->onCollied(contact, bodyB);
        node = (B2CCNode *)(bodyB->GetUserData());

        if(node != NULL)
            node->onCollied(contact, bodyA);
    }
    else
    {
        node = (B2CCNode *)(bodyB->GetUserData());
        if(node != NULL && TagHelper::Instance()->isObject(node->getTag(), ON_ROLE))
        {
            node->onCollied(contact, bodyA);

            node = (B2CCNode*)(bodyB->GetUserData());
            if(node != NULL)
                node->onCollied(contact, bodyA);
        }
    }





}

void B2Helper::bodiesListener(float dt)
{


	vector<b2Body*>::iterator iter;
	if(!m_sleepingPool->empty())
	{
		for(iter = m_sleepingPool->begin(); iter != m_sleepingPool->end(); ++iter)
		{
			(*iter)->SetActive(false);
		}
		m_sleepingPool->clear();
	}

	if(!m_deadPool->empty())
	{
		for (iter = m_deadPool->begin(); iter != m_deadPool->end(); ++iter)
		{
			m_world->DestroyBody(*iter);
		}
        m_deadPool->clear();
	}

    if(!m_activePool->empty())
    {
        for(iter = m_activePool->begin(); iter != m_activePool->end(); ++iter)
        {
            (*iter)->SetActive(true);
        }
        m_activePool->clear();
    }


    if(!m_jointPool->empty())
    {
        vector<b2JointDef*>::iterator iter;
        for(iter = m_jointPool->begin(); iter != m_jointPool->end(); ++iter)
        {
            m_world->CreateJoint(*iter);
        }
        m_jointPool->clear();
    }


}

void B2Helper::setMass(b2Body *body, int mass)
{
    b2MassData massData;
    massData.mass = mass;
    massData.center = b2Vec2(0, 0);
    massData.I = 0.0f;

    body->SetMassData(&massData);
}