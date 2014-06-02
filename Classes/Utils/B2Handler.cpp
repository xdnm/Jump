#include "B2Handler.h"

B2Handler::B2Handler()
{

}

B2Handler::B2Handler(b2World* world)
{
    world->SetContactListener(this);
}

B2Handler::~B2Handler()
{

}

B2Handler* B2Handler::createB2Handler(b2World *world)
{
    static B2Handler* instance = new B2Handler(world);
    return instance;
}

void B2Handler::BeginContact(b2Contact* contact)
{
    //colliedLog(contact);


}

void B2Handler::EndContact(b2Contact* contact)
{
    
}

void B2Handler::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
    //colliedLog(contact);
    colliedDispatcher(contact);
}

void B2Handler::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{

}

void B2Handler::colliedDispatcher(b2Contact *contact)
{
    b2Body *bodyA, *bodyB;
    bodyA = contact->GetFixtureA()->GetBody();
    bodyB = contact->GetFixtureB()->GetBody();

    CCNode *nodeA, *nodeB;
    nodeA = (CCNode *)(bodyA->GetUserData());
    nodeB = (CCNode *)(bodyB->GetUserData());

    //if there is a body has no userdata, do not handle this collision
    if(nodeA == NULL || nodeB == NULL)
        return;

    ////if this is a role collision
    //if(TagHelper::Instance()->isObject(nodeA->getTag(), ON_ROLE) || TagHelper::Instance()->isObject(nodeB->getTag(), ON_ROLE))
    //{
    //    RoleObject *role;
    //    if(TagHelper::Instance()->isObject(nodeA->getTag(), ON_ROLE))
    //    {
    //        role = dynamic_cast<RoleObject*>(nodeA);
    //        role->onCollied(contact, bodyB);
    //    }
    //    else
    //    {
    //        role = dynamic_cast<RoleObject*>(nodeB);
    //        role->onCollied(contact, bodyA);
    //    }
    //}

    //if this is a attacking collision.
    if(TagHelper::Instance()->isObject(nodeA->getTag(), ON_WEAPON)
        || TagHelper::Instance()->isObject(nodeB->getTag(), ON_WEAPON))
    {
        RoleObject *role;
        Weapon *weapon;
        if(TagHelper::Instance()->isObject(nodeA->getTag(), ON_WEAPON))
        {
            weapon = dynamic_cast<Weapon*>(nodeA);
            role = static_cast<RoleObject*>(weapon->userdata);
            role->onAttacking(contact, bodyB);
        }
        else
        {
            weapon = dynamic_cast<Weapon*>(nodeB);
            role = static_cast<RoleObject*>(weapon->userdata);
            role->onAttacking(contact, bodyA);
        }
        contact->SetEnabled(false);
    }
    else
    {
        B2CCNode *b2node;
        if(nodeA != NULL)
            dynamic_cast<B2CCNode*>(nodeA)->onCollied(contact, bodyB);

        if(nodeB != NULL)
            dynamic_cast<B2CCNode*>(nodeB)->onCollied(contact, bodyA);
    }


}
//void B2Handler::colliedDispatcher(b2Contact *contact)
//{
//
//    b2Body *bodyA, *bodyB;
//    bodyA = contact->GetFixtureA()->GetBody();
//    bodyB = contact->GetFixtureB()->GetBody();
//
//    B2CCNode *node = (B2CCNode*)(bodyA->GetUserData());
//    if(node != NULL&& TagHelper::Instance()->isObject(node->getTag(), ON_ROLE))
//    {
//        node->onCollied(contact, bodyB);
//        node = (B2CCNode *)(bodyB->GetUserData());
//
//        if(node != NULL)
//            node->onCollied(contact, bodyA);
//    }
//    else
//    {
//        node = (B2CCNode *)(bodyB->GetUserData());
//        if(node != NULL && TagHelper::Instance()->isObject(node->getTag(), ON_ROLE))
//        {
//            node->onCollied(contact, bodyA);
//
//            node = (B2CCNode*)(bodyB->GetUserData());
//            if(node != NULL)
//                node->onCollied(contact, bodyA);
//        }
//    }
//
//
//
//
//
//}
