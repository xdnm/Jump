#include "Pills.h"

Pill::Pill()
{
    this->setTag(TagHelper::Instance()->getTag(ON_PILLS));
}

Pill::~Pill()
{
    B2Helper::Instance()->putDeadPool(m_polyBody);
}

Pill* Pill::createPillWithType(PillType type, CCPoint position)
{
    Pill *pRet = new Pill();

    if(pRet != NULL && pRet->initPillWithType(type, position))
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

bool Pill::initPillWithType(PillType type, CCPoint position)
{
    CCNode::init();

    m_position = position;
    this->setPosition(m_position);
    m_pillType = type;
    m_size = CCSizeMake(60, 60);

    b2PolygonShape shape;
    shape.SetAsBox((m_size.width/2.0f) / PTM_RATIO, (m_size.height/2.0f) / PTM_RATIO);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.filter.categoryBits = BM_PILLS;
    fixtureDef.filter.maskBits = BM_ROLE | BM_PILLS;

    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position = b2Vec2(m_position.x / PTM_RATIO, m_position.y / PTM_RATIO);

    m_polyBody = B2Helper::Instance()->getWorld()->CreateBody(&bodyDef);
    m_polyBody->CreateFixture(&fixtureDef);

    m_polyBody->SetUserData(this);

    if(m_pillType == PT_RED)
    {
        m_visiableNode = CCSprite::create("redpill.png");
    }
    else if(m_pillType == PT_GREEN)
    {
        m_visiableNode = CCSprite::create("greenpill.png");
    }
    else if(m_pillType == PT_YELLOW)
    {
        m_visiableNode = CCSprite::create("yellowpill.png");
    }
    else if(m_pillType == PT_BLUE)
    {
        m_visiableNode = CCSprite::create("bluepill.png");
    }

    if(m_visiableNode != NULL)
    {
        m_visiableNode->setScaleX(m_size.width / m_visiableNode->getContentSize().width);
        m_visiableNode->setScaleY(m_size.height / m_visiableNode->getContentSize().height);
        //m_visiableNode->setPosition(m_position);
        m_visiableNode->setZOrder(10);
        this->addChild(m_visiableNode);
    }

    this->schedule(schedule_selector(Pill::releaseUpdate), 0.1f);
    //this->release();


    CCActionInterval *rotate = CCRotateTo::create(0.05f, 15.0f, 15.0f);
    CCActionInterval *rotateBack = CCRotateTo::create(0.05, -15.0f, -15.0f);
   
    CCActionInterval *innerRep = CCRepeat::create(CCSequence::create(rotate, rotateBack, NULL), 5);
    CCActionInterval *delay = CCDelayTime::create(1.0f);

    CCAction *rep = CCRepeatForever::create(CCSequence::create(innerRep,delay, NULL));

    this->runAction(rep);
    return true;
}

void Pill::releaseUpdate(float dt)
{
    int yPosition = convertToWorldSpace(ccp(0, 0)).y;
    CCLog("pill position %f", yPosition);
    if( yPosition < -100 || yPosition > 1500)
    {
        this->removeFromParent();
    }
}

void Pill::onCollied(b2Contact* contact, b2Body *bodyOther)
{

}