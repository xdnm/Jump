#include "Object/RigidEdge.h"
#include "Utils/B2WorldHelper.h"

RigidEdge::RigidEdge()
{
    this->setTag(TagHelper::Instance()->getTag(ON_EDGE));
}

RigidEdge::~RigidEdge()
{
    B2Helper::Instance()->putDeadPool(m_polyBody);
}

RigidEdge* RigidEdge::createRigidEdge(CCPoint position, int height, void *parm)
{
    RigidEdge *pRet = new RigidEdge();
    if(pRet && pRet->initWithDockDir(position, height, parm))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool RigidEdge::initWithDockDir(CCPoint position, int height, void *parm)
{
    m_position = position;

    m_size = CCSizeMake(2, height);

    b2PolygonShape polyShape;
    polyShape.SetAsBox(m_size.width/PTM_RATIO * 0.5, m_size.height/PTM_RATIO * 0.5);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &polyShape;
    fixtureDef.restitution = 0.0f;
    fixtureDef.filter.categoryBits = BM_EDGE;


    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.userData = this;
    bodyDef.position = b2Vec2(m_position.x / PTM_RATIO, m_position.y/PTM_RATIO);

    m_polyBody = B2Helper::Instance()->getWorld()->CreateBody(&bodyDef);
    m_polyBody->CreateFixture(&fixtureDef);

    return true;
}