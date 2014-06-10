#include "Weapon/SwordWeapon.h"

SwordWeapon::SwordWeapon()
{

}

SwordWeapon::~SwordWeapon()
{
    delete m_wave1;
}

bool SwordWeapon::initModel()
{
    m_model = new ObjectModel();
    m_model->m_damege = 1;
    m_model->m_hitRatio = 1.0f;

    return true;
}

SwordWeapon* SwordWeapon::createSword(CCNode *hand, float unit,b2Body *hookRoleBody, void *parm)
{
    SwordWeapon *pRet = new SwordWeapon();

    if(pRet && pRet->initWithParm(hand, unit,hookRoleBody, parm))
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

bool SwordWeapon::initWithParm(CCNode *hand, float unit /* = 1.0f */,b2Body *hookRoleBody, void *parm)
{
    //we need to get the parm data to init the weapon
    m_hand = hand;

    //testing code for init, I hard code the parm in below region
    m_unit = unit;
    m_size = CCSizeMake(30, 90);
    m_texture = cocos2d::CCTextureCache::sharedTextureCache()->addImage("sword1.png");
    m_wave1 = NULL;

    //init the default position and rotation
    m_defaultPosiion = ccp(0, -0.5f * m_unit);
    m_defaultRotation =0.0f;

    if(hookRoleBody != NULL)
        m_hookRoleBody = hookRoleBody;

    m_hookedBody = NULL;
    m_jointDef = NULL;

    initActions();

    this->setScale(m_size.width/m_texture->getContentSize().width);

    m_damage = 1;
    m_criticalRate = 0.05f;
    //end testing code

    this->initWithTexture(m_texture);
   // this->initWithFile("sword3.png"); 

    this->setAnchorPoint(ccp(0.5f,0));

    this->setPosition(m_defaultPosiion);
    this->setRotation(m_defaultRotation);


    this->scheduleUpdate();
    
    return true;
}

bool SwordWeapon::hook(b2Contact *contact, b2Body *otherBody)
{
    //return if this weapon is already hook to a body.
    if(m_isHooked)
        return false;

    CCLog("began hook!");
    if(m_onAttacking)
        endDamege();
    //endDamege();
    //endAttack();
    m_isHooked = true;
    m_hookedBody = otherBody;
    
    //remove this weapon from hand, use the hookBody and hookedBody updating its position and rotation.
    m_hand->removeChild(this, false);
    this->setPosition(ccp(0, 0));
    this->setRotation(0.0f);
    userdata->addChild(this);
    
    m_jointDef= new b2DistanceJointDef();
    b2WorldManifold *maniFold = new b2WorldManifold();
    contact->GetWorldManifold(maniFold);
    m_jointDef->collideConnected = true;
    m_jointDef->dampingRatio = 0.1f;
    m_jointDef->frequencyHz = 4.0f;
    m_jointDef->Initialize(m_hookRoleBody, otherBody, m_hookRoleBody->GetWorldCenter(), maniFold->points[0]);
    B2Helper::Instance()->putJointPool(m_jointDef);

    this->schedule(schedule_selector(SwordWeapon::updateWeapon));
    delete maniFold;
    return true;
}
bool SwordWeapon::endHook()
{
    if(!m_isHooked)
        return false;
    
    CCLog("end hook!");
    m_isHooked = false;
    B2Helper::Instance()->putDestroyJointPool(m_jointDef);

    this->userdata->removeChild(this);
    
    this->setPosition(m_defaultPosiion);
    this->setRotation(m_defaultRotation);

    m_hand->addChild(this);
    
    this->unschedule(schedule_selector(SwordWeapon::updateWeapon));
    return true;
}

bool SwordWeapon::initActions()
{
    m_wave1 = new ActionGroup();
    AttackAction *action, *nextAction;
    action = new AttackAction();
    action->moveDur = 0.1f;
    action->moveToPos = ccp(1.0f * m_unit , 0.5f * m_unit);
    action->rotation = 30.0f;
    action->rotationDur = 0.1f;
    action->postAction = new AttackAction();
    nextAction = action->postAction;
    
    nextAction->moveDur = 0.1f;
    nextAction->moveToPos = ccp(0, 0);
    nextAction->rotation = -90.0f;
    nextAction->rotationDur = 0.1f;
    nextAction->postAction = new AttackAction();
    nextAction = nextAction->postAction;

    nextAction->moveToPos = m_defaultPosiion;
    nextAction->moveDur = 0.1f;
    nextAction->rotation = m_defaultRotation;
    nextAction->rotationDur = 0.1f;

    nextAction->postAction = NULL;

    //init the attacking area shape for m_wave1;
    b2PolygonShape shape;
    shape.SetAsBox(m_size.height/PTM_RATIO * 0.5f, m_size.height/PTM_RATIO * 0.5f);
    m_wave1->attackShape = shape;
    m_wave1->actionList = action;
    m_wave1->xOffset = m_size.height/2;
    createActions();

    return true;
}
bool SwordWeapon::attackAction()
{
    if(m_wave1 == NULL)
        initActions();


    if(!tryBeginAttack())
        return false;

    if(m_isFaceLeft)
    {
        this->stopAllActions();
        this->runAction(m_wave1->m_leftVersion);

    }
    else
    {
        this->stopAllActions();
        this->runAction(m_wave1->m_rightVersion);
    }



    return true;
}

void SwordWeapon::createActions()
{
    int leftFlag = 1;
   
    CCCallFunc *beginDamege = CCCallFunc::create(this, callfunc_selector(SwordWeapon::beginDamege));
    CCCallFunc *endDamege    = CCCallFunc::create(this, callfunc_selector(SwordWeapon::endDamege));
    CCCallFunc *endAttack       = CCCallFunc::create(this, callfunc_selector(SwordWeapon::endAttack));

    //for now, we only have one attack action m_wave1 for our swordweapon;
    //create the attacking weapon movement.
    for(int i = 0; i < 2; i++)
    {
        if(i == 0)
            leftFlag = 1;
        else 
            leftFlag = -1;

        AttackAction *action1 = m_wave1->actionList;
        CCArray *actionArray = CCArray::create();
        int j = 0;
        while(action1 != NULL)
        {
            CCFiniteTimeAction *Spawn;
            CCFiniteTimeAction *tempSeq;
            CCActionInterval *rotation = CCRotateTo::create(action1->rotationDur, leftFlag * action1->rotation);
            CCActionInterval *move = CCMoveTo::create(action1->moveDur, ccp(action1->moveToPos.x * leftFlag, action1->moveToPos.y));
            if(j == 0)
            {
                Spawn = CCSpawn::create(rotation, move, NULL);
                tempSeq = CCSequence::create(Spawn, beginDamege, NULL);

            }
            else if(action1->postAction == NULL)
            {
                Spawn = CCSpawn::create(move, rotation,NULL);
                tempSeq = CCSequence::create(endDamege, Spawn, endAttack, NULL);
            }
            else 
            {
                Spawn = CCSpawn::create(move, rotation, NULL);
                tempSeq = CCSequence::create(Spawn, NULL);
            }

            actionArray->addObject(tempSeq);
            action1 = action1->postAction;
            j++;
        }
        
        CCFiniteTimeAction *seq = CCSequence::create(actionArray);
        seq->retain();
        if(leftFlag == 1)
            m_wave1->m_leftVersion = seq;
        else
            m_wave1->m_rightVersion = seq;

    }

}

void SwordWeapon::beginDamege()
{
    CCPoint point = this->getWeaponPosition();

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &(m_wave1->attackShape);
    fixtureDef.filter.categoryBits = BM_WEAPON;
    fixtureDef.filter.maskBits = BM_EDGE | BM_BLOCK;

    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(point.x/PTM_RATIO, point.y/PTM_RATIO);
    bodyDef.type = b2_dynamicBody;
    bodyDef.fixedRotation = true;
    bodyDef.gravityScale = 0.0f;

    m_attackAreaBody = B2Helper::Instance()->getWorld()->CreateBody(&bodyDef);
    m_attackAreaBody->CreateFixture(&fixtureDef);

    m_attackAreaBody->SetUserData(this);

    this->schedule(schedule_selector(SwordWeapon::updateAttackArea));
    CCLog("begin damege");
}

void SwordWeapon::endDamege()
{
    if(!m_onAttacking)
        return;
    CCLog("end damege");
    this->unschedule(schedule_selector(SwordWeapon::updateAttackArea));
    if(m_attackAreaBody != NULL)
        B2Helper::Instance()->putDeadPool(m_attackAreaBody);

    m_onAttacking = false;
    clearAttackPool();
}

void SwordWeapon::update(float delta)
{
    /*          CCLog("sword position : (%f, %f)", getPositionX(), getPositionY());
    if(m_isHooked)
    {
    CCPoint anchorPoint = ccp(m_hookRoleBody->GetPosition().x * PTM_RATIO, m_hookRoleBody->GetPosition().y *PTM_RATIO);
    CCPoint hookedPoint = ccp(m_hookedBody->GetPosition().x * PTM_RATIO, m_hookedBody->GetPosition().y * PTM_RATIO);


    this->setPosition(anchorPoint);
    float angle = ccpAngle(hookedPoint - anchorPoint, ccp(0, 1));

    if(anchorPoint.x > hookedPoint.x)
    angle = -angle;

    this->setRotation(CC_RADIANS_TO_DEGREES(angle));

    }*/
}

void SwordWeapon::updateAttackArea(float delta)
{
    //update the attacking area position.
    //this is a sword weapon, so the attacking area aways follow the hand.
    if(m_attackAreaBody != NULL && B2Helper::Instance()->getWorld()->IsLocked() == false)
    {
        CCPoint point = this->getWeaponPosition();
        m_attackAreaBody->SetTransform(b2Vec2(point.x / PTM_RATIO, point.y / PTM_RATIO), 0);
    }
}

void SwordWeapon::updateWeapon(float dt)
{
    if(m_isHooked)
    {
        CCPoint anchorPoint = ccp(m_hookRoleBody->GetPosition().x * PTM_RATIO, m_hookRoleBody->GetPosition().y *PTM_RATIO);
        CCPoint hookedPoint = ccp(m_hookedBody->GetPosition().x * PTM_RATIO, m_hookedBody->GetPosition().y * PTM_RATIO);


        this->setPosition(anchorPoint);
        float angle = ccpAngle(hookedPoint - anchorPoint, ccp(0, 1));

        if(anchorPoint.x > hookedPoint.x)
            angle = -angle;

        this->setRotation(CC_RADIANS_TO_DEGREES(angle));

    }
}

BiliBoard* SwordWeapon::interationWithOther(b2Contact *contact, b2Body* otherBody, bool onTouching)
{
    CCNode *othernode = static_cast<CCNode*>(otherBody->GetUserData());

    if(othernode == NULL)
        return NULL;
    if(tryPushAttackPool(othernode))
    {
        CCLog("role interact with tag: %d", othernode->getTag());

        if(TagHelper::Instance()->isObject(othernode->getTag(), ON_BLOCK) && isHooked() == false)
        {
            if(onTouching)
                hook(contact, otherBody);
        }
    }

    return NULL;
}




