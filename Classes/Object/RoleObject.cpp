#include "RoleObject.h"

RoleObject::RoleObject()
{
    this->setTag(TagHelper::Instance()->getTag(ON_ROLE));
    
    //m_normalFaceDir = ""
}
RoleObject::~RoleObject()
{
    if(m_model != NULL)
        delete m_model;

    B2Helper::Instance()->putDeadPool(m_bottomBody);
    B2Helper::Instance()->putDeadPool(m_innerBody);
    B2Helper::Instance()->putDeadPool(m_topBoby);
    B2Helper::Instance()->putDeadPool(m_leftBody);
    B2Helper::Instance()->putDeadPool(m_rightBody);

    this->removeAllChildren();

    this->unscheduleAllSelectors();
}

RoleObject* RoleObject::CreateRole(b2World* world, void *parm)
{
    RoleObject *pRet = new RoleObject();

    if(pRet && pRet->initWithWorld(world, parm))
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

bool RoleObject::initWithWorld(b2World* world, void *parm)
{
	bool ret = true;
	do 
	{
		if(!CCNode::init())
		{
			ret = false;
			break;
		}

       m_onTouchDown = false;
        //init the length first, and the face direction
        m_length = 50.0f;
        m_Unit = m_length;
        m_faceLeft = true;
        m_isFlying = false;

		//fileds initialize part
		m_innerBody = NULL;
		m_leftBody = NULL;
		m_rightBody = NULL;
		m_topBoby = NULL;
		m_bottomBody = NULL;

		m_position = ccp(0, 0);

		m_innerRadius = 0.0f;
		m_outRadius = 0.0f;
		//end fields initialize part
        
        m_acceleration.x = 0.0;
		//world init

        m_world = world;
		m_position = ccp(200, 400);

        m_bounce = false;

        m_visiableNode = CCSprite::create();
        this->addChild(m_visiableNode);



        m_leftHandSprite = CCNode::create();
        m_rightHandSprite = CCNode::create();
        //m_leftHandSprite->retain();
        //m_rightHandSprite->retain();
        //m_leftHandSprite->setScale(30/m_leftHandSprite->getContentSize().width);
        //m_rightHandSprite->setScale(40/m_leftHandSprite->getContentSize().width);
        this->addChild(m_leftHandSprite);
        this->addChild(m_rightHandSprite);
        
        m_face = CCSprite::create("face_smile.png");
        m_face->setScale(m_length * 2 / m_face->getContentSize().width);

        this->m_visiableNode->addChild(m_face);

		//render date init
		m_textureCoords[0] = Vertex2DMake(0.0f, 0.0f);
		m_textureCoords[1] = Vertex2DMake(1.0f, 0.0f);
		m_textureCoords[2] = Vertex2DMake(0.0f, 0.5f);
		m_textureCoords[3] = Vertex2DMake(1.0f, 0.5f);
		m_textureCoords[4] = Vertex2DMake(0.0f, 1.0f);
		m_textureCoords[5] = Vertex2DMake(1.0f, 1.0f);
		m_texture = cocos2d::CCTextureCache::sharedTextureCache()->addImage("role_background.png");
        cocos2d::CCTextureCache::sharedTextureCache()->addImage("face_aggressive.png");
        cocos2d::CCTextureCache::sharedTextureCache()->addImage("face_amazing.png");
        cocos2d::CCTextureCache::sharedTextureCache()->addImage("face_pain.png");
        cocos2d::CCTextureCache::sharedTextureCache()->addImage("face_smile.png");
        cocos2d::CCTextureCache::sharedTextureCache()->addImage("face_uppset.png");

        
        //cocos2d::CCTextureCache::sharedTextureCache()->addImage("role_background");
        
        
		//create the pysical body 
		createBody(parm);


        initModel();

        //create the weapon and init the unit width of the weapon.
        m_weapon = SwordWeapon::createSword(m_leftHandSprite, m_Unit, m_innerBody, NULL);
        m_leftHandSprite->addChild(m_weapon);
        //this->addChild(m_weapon);
        m_weapon->userdata = this;

        m_model->plusAModel(m_weapon->m_model);
        m_weapon->m_roleModel = m_model;

        //m_leftHandSprite->setFlipY(true);
  
        //after the weapon attach to role, we can init the role's biliboard
        

        setFaceLeft(true);
        this->scheduleUpdate();
        this->schedule(schedule_selector(RoleObject::updateAcceleration), 0.1f);
        //B2Helper::Instance()->getWorld()->SetContactListener(this);
  
        m_isProtected = false;
        m_isJumping = false;

        m_blockColliedFlag = false;
        m_blockCollied = NULL;
		break;
	} while (1);

	return ret;
}

bool RoleObject::initModel()
{
    m_model = new ObjectModel();
    m_model->d_damege = 5;
    m_model->d_defence = 10;
    m_model->d_health = 10;
    m_model->d_hitRatio = 5;
    m_model->d_parry = 5;
   

    m_model->setStrong(5);
    m_model->setStrength(5);
    m_model->setAgility(5);
    m_model->setFirm(5);

    //body sprite init


    this->m_mainBody = m_innerBody;
    m_model->m_B2Node = this;
   
    return true;
}

void RoleObject::createBody(void *parm)
{
    //the parm is null in temporary.

    //body parm init

    m_outRadius = m_length;
    m_innerRadius = m_length / 3;




    float blockWidth = 3.0f/2;
    float blockLength = m_length/2;
    //end body parm init;

    b2CircleShape circleShape;
    circleShape.m_radius = m_innerRadius/PTM_RATIO;

    b2PolygonShape polyShapeHorizon;
    float temp = blockLength/PTM_RATIO;
    polyShapeHorizon.SetAsBox(blockLength/PTM_RATIO, blockWidth/PTM_RATIO);
    b2PolygonShape polyShapeVerticle;
    polyShapeVerticle.SetAsBox(blockWidth/PTM_RATIO, blockLength/PTM_RATIO);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &polyShapeHorizon;
    fixtureDef.density = 10;
    fixtureDef.restitution = 0.5f;
    fixtureDef.friction = 1.0f;
    fixtureDef.filter.categoryBits = BM_ROLE;
    fixtureDef.filter.maskBits = BM_MONSTOR | BM_BLOCK | BM_EDGE;


    b2BodyDef bodyDef;

    bodyDef.type	 = b2_dynamicBody;
    bodyDef.position = b2Vec2(m_position.x/PTM_RATIO, (m_position.y + m_outRadius)/PTM_RATIO);
    bodyDef.fixedRotation = true;

    //create top block;
    m_topBoby = m_world->CreateBody(&bodyDef);
    m_topBoby->CreateFixture(&fixtureDef);

    //create bottom block
    polyShapeHorizon.SetAsBox((blockLength*2)/PTM_RATIO, blockWidth/PTM_RATIO);
    bodyDef.position = b2Vec2(m_position.x/PTM_RATIO, (m_position.y - m_outRadius)/PTM_RATIO);
    m_bottomBody = m_world->CreateBody(&bodyDef);
    fixtureDef.filter.groupIndex = 2;
    m_bottomBody->CreateFixture(&fixtureDef);
    m_bottomBody->SetBullet(true);

    //create left block
    fixtureDef.shape = &polyShapeVerticle;
    bodyDef.position = b2Vec2((m_position.x - m_outRadius)/PTM_RATIO, m_position.y/PTM_RATIO);
    m_leftBody = m_world->CreateBody(&bodyDef);
    m_leftBody->CreateFixture(&fixtureDef);

    //create right block
    bodyDef.position = b2Vec2((m_position.x + m_outRadius)/PTM_RATIO, m_position.y/PTM_RATIO);
    m_rightBody = m_world->CreateBody(&bodyDef);
    m_rightBody->CreateFixture(&fixtureDef);

    //create inner circle
    fixtureDef.shape = &circleShape;
    bodyDef.position = b2Vec2(m_position.x/PTM_RATIO, m_position.y/PTM_RATIO);


    m_innerBody = m_world->CreateBody(&bodyDef);
    m_innerBody->CreateFixture(&fixtureDef);

    //old way to create the body
    //createDistanceJoint(m_innerBody, m_leftBody);
    //createDistanceJoint(m_innerBody, m_rightBody);
    //createDistanceJoint(m_innerBody, m_topBoby);
    //createDistanceJoint(m_innerBody, m_bottomBody);

    //createRevoluteJoint(m_innerBody, m_leftBody, 15.0f);
    //createRevoluteJoint(m_innerBody, m_rightBody, 15.0f);
    //createRevoluteJoint(m_innerBody, m_topBoby, 15.0f);
    //createRevoluteJoint(m_innerBody, m_bottomBody, 15.0f);

    //new way to creat the body
    createDistanceJoint(m_innerBody, m_topBoby);
    createDistanceJoint(m_innerBody, m_bottomBody, 0.2f, 4.0f);
    createDistanceJoint(m_innerBody, m_leftBody, 1.0f, 4.0f);
    createDistanceJoint(m_innerBody, m_rightBody, 1.0f, 4.0f);

    createDistanceJoint(m_topBoby, m_leftBody, 0.8f, 4.0f);
    createDistanceJoint(m_topBoby, m_rightBody, 0.8f, 4.0f);
    createDistanceJoint(m_bottomBody, m_rightBody);
    createDistanceJoint(m_bottomBody, m_leftBody);
    createDistanceJoint(m_leftBody, m_rightBody);

    createRevoluteJoint(m_innerBody, m_topBoby, 15.0f);
    createRevoluteJoint(m_innerBody, m_bottomBody, 15.0f);

    //test for create the joints between bottom body and left&right body
    b2DistanceJointDef distanceJointDef;
    distanceJointDef.dampingRatio = 1.0f;
    distanceJointDef.frequencyHz = 4.0f;
    distanceJointDef.Initialize(m_bottomBody, m_leftBody, m_bottomBody->GetWorldCenter() + b2Vec2(-blockLength*2/PTM_RATIO, 0), m_leftBody->GetWorldCenter());
    m_world->CreateJoint(&distanceJointDef);

    distanceJointDef.Initialize(m_bottomBody, m_rightBody, m_bottomBody->GetWorldCenter() + b2Vec2(blockLength*2/PTM_RATIO, 0), m_rightBody->GetWorldCenter());
    m_world->CreateJoint(&distanceJointDef);
    /*createRevoluteJoint(m_innerBody, m_leftBody, 30.0f);
    createRevoluteJoint(m_innerBody, m_rightBody, 30.0f);*/

    //set userdata of  each part of the body
    m_leftBody->SetUserData((RoleObject*)this);
    m_rightBody->SetUserData((RoleObject*)this);
    m_topBoby->SetUserData((RoleObject*)this);
    m_bottomBody->SetUserData((RoleObject*)this);
    m_innerBody->SetUserData((RoleObject*)this);

    //set mass for each part of the body
    B2Helper::Instance()->setMass(m_innerBody, 10);
    B2Helper::Instance()->setMass(m_bottomBody, 2);
    B2Helper::Instance()->setMass(m_leftBody, 0.1);
    B2Helper::Instance()->setMass(m_rightBody, 0.1);

}

void RoleObject::createDistanceJoint(b2Body *bodyA, b2Body *bodyB, float dampRatio /* = 0.5f */, float freq /* = 4.0f */)
{
	if(bodyA == NULL || bodyB == NULL)
		return;

	b2DistanceJointDef jointDef;
	jointDef.Initialize(bodyA, bodyB, bodyA->GetWorldCenter(), bodyB->GetWorldCenter());
	jointDef.collideConnected = true;
	jointDef.frequencyHz = freq;
	jointDef.dampingRatio = dampRatio;

	m_world->CreateJoint(&jointDef);
   
}
void RoleObject::createRevoluteJoint(b2Body *bodyA, b2Body *bodyB, float angle)
{
	if(bodyA == NULL || bodyB == NULL)
		return;

    b2RevoluteJointDef jointDef;
	jointDef.bodyA = bodyA;
	jointDef.bodyB = bodyB;
	jointDef.collideConnected = true;
	jointDef.localAnchorA = bodyA->GetWorldCenter();
	jointDef.localAnchorB = bodyB->GetWorldCenter();
	jointDef.enableMotor = false;
	jointDef.enableLimit = false;
	jointDef.lowerAngle = CC_DEGREES_TO_RADIANS(-angle);
	jointDef.upperAngle = CC_DEGREES_TO_RADIANS(angle);
	jointDef.Initialize(bodyA, bodyB, bodyA->GetWorldCenter());

	m_world->CreateJoint(&jointDef);

}

void RoleObject::jump(float massRatio /* = 1.0f */)
{
	if(m_innerBody == NULL)
		return;
	//m_->SetLinearVelocity(b2Vec2(0, 0));
	b2Vec2 impulse = b2Vec2(0.0f, m_innerBody->GetMass() * massRatio);
    
    m_bounce = true;
    m_isFlying = false;
	//m_bottomBody->ApplyLinearImpulse(impulse, m_bottomBody->GetWorldCenter());
    //

    
}

void RoleObject::draw()
{
	m_vertexCoords[0] = Vertex2DMake(m_leftBody->GetPosition().x * PTM_RATIO, m_topBoby->GetPosition().y * PTM_RATIO);
	m_vertexCoords[1] = Vertex2DMake(m_rightBody->GetPosition().x * PTM_RATIO, m_topBoby->GetPosition().y * PTM_RATIO);
	m_vertexCoords[2] = Vertex2DMake(m_leftBody->GetPosition().x * PTM_RATIO, m_leftBody->GetPosition().y * PTM_RATIO);
	m_vertexCoords[3] = Vertex2DMake(m_rightBody->GetPosition().x * PTM_RATIO, m_rightBody->GetPosition().y * PTM_RATIO);
	m_vertexCoords[4] = Vertex2DMake(m_leftBody->GetPosition().x * PTM_RATIO, m_bottomBody->GetPosition().y * PTM_RATIO);
	m_vertexCoords[5] = Vertex2DMake(m_rightBody->GetPosition().x * PTM_RATIO, m_bottomBody->GetPosition().y * PTM_RATIO);

	ccGLBindTexture2D(m_texture->getName());
	m_texture->getShaderProgram()->use();
	m_texture->getShaderProgram()->setUniformsForBuiltins();
	ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords);
	glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, m_textureCoords);
	glVertexAttribPointer(kCCVertexAttrib_Position,  2, GL_FLOAT, GL_FALSE, 0, m_vertexCoords);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);

}

//old collision function
//void RoleObject::onCollied(b2Contact *contact, b2Body *bodyOther)
//{
//    if(!contact->IsEnabled())
//        return ;
//
//	CCNode *otherNode = (CCNode*)(bodyOther->GetUserData());
//    b2Body *selfBody;
//    if(contact->GetFixtureA()->GetBody() == bodyOther)
//        selfBody = contact->GetFixtureA()->GetBody();
//    else
//        selfBody = contact->GetFixtureB()->GetBody();
//	//CCLog("tag %d collied with tag %d", this->getTag(), otherNode == NULL? 0:otherNode->getTag());
//
//    //if the othernode is a unset object or the edge object, do nothing to this collision
//    if(otherNode == NULL)
//        return ;
//    else if(otherNode->getTag() == 3)
//        return;
//    
//    //if the role is moving up, ingore this collision
//    if(m_innerBody->GetLinearVelocity().y > 0 || m_weapon->isHooked())
//    {
//        if(TagHelper::Instance()->isObject(otherNode->getTag(), ON_MONSTOR) )
//        {
//            if(m_bottomBody->GetPosition().y < bodyOther->GetPosition().y)
//            {
//                Monster *monster = dynamic_cast<Monster*>(otherNode);
//                beenAttacked(monster);
//                
//
//                contact->SetEnabled(false);
//            }
//            else
//            {
//                contact->SetEnabled(true);
//            }
//        }
//        else
//        {
//            contact->SetEnabled(false);
//        }
//    }
//    else if(bodyOther->GetPosition().y > m_bottomBody->GetPosition().y)
//    {
//        //if role is moving down, but the colliision object is above at the bottom body, ingore this collision.
//        if(TagHelper::Instance()->isObject(otherNode->getTag(), ON_MONSTOR) 
//            && m_bottomBody->GetPosition().y < bodyOther->GetPosition().y)
//        {
//            Monster *monster = dynamic_cast<Monster*>(otherNode);
//
//            monster->attacked(m_model);
//        }
//        contact->SetEnabled(false);
//    }
//    else
//    {
//        if(TagHelper::Instance()->isObject(otherNode->getTag(), ON_BLOCK))
//            jump(20.0f);
//
//        if(TagHelper::Instance()->isObject(otherNode->getTag(), ON_MONSTOR))
//        {
//            
//            Monster *monster = dynamic_cast<Monster*>(otherNode);
//            if(monster->isReady())
//            {
//                jump(10.0f);
//                monster->beenTrampled(m_model);
//            }
//        }
//        contact->SetEnabled(true);
//    }
//}



void RoleObject::onCollied(b2Contact *contact, b2Body *bodyOther)
{
    bool isEnable = true;
    do 
    {
        B2CCNode *otherNode;
        otherNode = static_cast<B2CCNode*>(bodyOther->GetUserData());

        b2Body *selfBody;
        if(contact->GetFixtureA()->GetBody() == bodyOther)
            selfBody = contact->GetFixtureB()->GetBody();
        else
            selfBody = contact->GetFixtureA()->GetBody();

        if(m_weapon->isHooked())
        {
            isEnable = false;
            break;
        }

        //collistion with a block
        //two choice: 
        //1: role is moving up, enable = false.
        //2. role is moving down and collisition happen by bottom body, enable = true;
        if(TagHelper::Instance()->isObject(otherNode->getTag(), ON_BLOCK))
        {
            if(selfBody == m_bottomBody && selfBody->GetPosition().y >= bodyOther->GetPosition().y)
            {
                if(m_innerBody->GetLinearVelocity().y < 0.0f)
                    jump();
                isEnable = true;
                break;
            }
            else
            {
                isEnable = false;
                break;
            }
            /*if(m_blockColliedFlag && m_blockCollied == bodyOther)
            {
            isEnable = true;
            break;
            }
            else if(selfBody == m_bottomBody && selfBody->GetPosition().y > bodyOther->GetPosition().y && m_innerBody->GetLinearVelocity().y <= 0.0f)
            {
            m_blockColliedFlag = true;
            m_blockCollied = bodyOther;

            CCActionInterval *delay = CCDelayTime::create(0.5f);
            CCCallFunc *callfunc = CCCallFunc::create(this,callfunc_selector(RoleObject::setColliedFlag));

            CCFiniteTimeAction *seq = CCSequence::create(delay, callfunc, NULL);


            this->runAction(seq);

            jump();
            isEnable = true;
            break;
            }
            else
            {
            isEnable = false;
            break;
            }*/
           
        }
        else if(TagHelper::Instance()->isObject(otherNode->getTag(), ON_MONSTOR))
        {
            Monster *monster = dynamic_cast<Monster*>(otherNode);
            CC_BREAK_IF(monster == NULL);

            float dis = abs(selfBody->GetPosition().y - bodyOther->GetPosition().y);

            if(selfBody == m_bottomBody && dis < 0.5f)
            {
                if(monster->isReady())
                {
                    monster->beenTrampled(m_model);
                    //jump();
                }
            }
            else 
            {
                if(m_faceLeft && m_innerBody->GetPosition().x < bodyOther->GetPosition().x)
                {
                    this->attack();
                }
                else if(!m_faceLeft && m_innerBody->GetPosition().x > bodyOther->GetPosition().x)
                {
                    this->attack();
                }
                else
                    this->beenAttacked(monster);
            }

            isEnable = false;
            break;
        }

        if(m_innerBody->GetPosition().y * PTM_RATIO <= m_length + 5)
        {
            m_innerBody->SetLinearVelocity(b2Vec2(0, 30));
        }
    } while (0);

    contact->SetEnabled(isEnable);


}
void RoleObject::update(float delta)
{
    if(!m_weapon->isHooked())
        passingBorder(m_innerBody->GetPosition().x * PTM_RATIO);

    m_visiableNode->setPosition(ccp(m_innerBody->GetPosition().x * PTM_RATIO, m_innerBody->GetPosition().y * PTM_RATIO));

    m_position = ccp(m_innerBody->GetPosition().x * PTM_RATIO, m_innerBody->GetPosition().y * PTM_RATIO);

    m_leftHandSprite->setPosition(ccp((m_leftBody->GetPosition().x) * PTM_RATIO, m_rightBody->GetPosition().y * PTM_RATIO));
    m_rightHandSprite->setPosition(ccp(m_rightBody->GetPosition().x * PTM_RATIO, m_rightBody->GetPosition().y * PTM_RATIO));
    
    b2Vec2 direct = m_leftBody->GetPosition() - m_innerBody->GetPosition();
    float radians = ccpAngle(ccp(direct.x, direct.y), ccp(0, -1));

    if(m_leftBody->GetPosition().x > m_innerBody->GetPosition().x)
        m_leftHandSprite->setRotation(90 - CC_RADIANS_TO_DEGREES(radians));
    else
        m_leftHandSprite->setRotation(CC_RADIANS_TO_DEGREES(radians) - 90);

    direct = m_rightBody->GetPosition() - m_innerBody->GetPosition();
    radians = ccpAngle(ccp(direct.x, direct.y), ccp(0, -1));
    if(m_rightBody->GetPosition().x > m_innerBody->GetPosition().x)
        m_rightHandSprite->setRotation(90 - CC_RADIANS_TO_DEGREES(radians));
    else
        m_rightHandSprite->setRotation(CC_RADIANS_TO_DEGREES(radians) - 90);

    if(m_innerBody->GetPosition().y * PTM_RATIO <= m_length + 10)
    {
        m_innerBody->ApplyLinearImpulse(b2Vec2(0.0f, m_innerBody->GetMass() * 1000), m_innerBody->GetWorldCenter());
        tryLaunchParticle(delta);
        GUILayer::Instance()->clearRolePower();
        m_bounce = false;
    }

    if(m_bounce)
    {
        if(abs(m_innerBody->GetLinearVelocity().y) < 5)
        {
            //this 100 is magic num which should be same with the max power defined in the PowerColum
            if(GUILayer::Instance()->getRolePower() < 100)
            {
                m_innerBody->ApplyLinearImpulse(b2Vec2(0.0f, m_innerBody->GetMass() * 50), m_innerBody->GetWorldCenter());
                m_isJumping = true;
                //tryLaunchParticle(ccp(m_bottomBody->GetPosition().x * PTM_RATIO, m_bottomBody->GetPosition().y * PTM_RATIO));
                //m_emitPoint = ccp(m_bottomBody->GetPosition().x * PTM_RATIO, m_bottomBody->GetPosition().y * PTM_RATIO);
                //this->scheduleOnce(schedule_selector(RoleObject::tryLaunchParticle), 0.1f);
                tryLaunchParticle(delta);

                GUILayer::Instance()->onRoleJumping();
                m_bounce = false;
            }
            else if(GUILayer::Instance()->getRolePower() >= 100)
            {
                m_innerBody->ApplyLinearImpulse(b2Vec2(0.0f, m_innerBody->GetMass() * 1000), m_innerBody->GetWorldCenter());
                tryLaunchParticle(delta);
                GUILayer::Instance()->clearRolePower();
                m_bounce = false;
                m_isJumping = true;

            }
            
        }
    }
    
}

bool RoleObject::attack()
{
    runChangeFaceAction("face_aggressive.png", 2.0f);
    m_weapon->attackAction();
    jump(5.0f);
    //setFaceLeft(!m_faceLeft);
    return true;
}

void RoleObject::hookAction(b2Body *hookedBody, b2Vec2 point)
{
    m_innerBody->SetLinearVelocity(b2Vec2(0, -10.0f));
    //plus the hook point
    CCPoint hookPoint = m_weapon->getWeaponPosition();
    
    //TODO Delete jointDef.
    b2DistanceJointDef *jointDef = new b2DistanceJointDef();
    jointDef->collideConnected = true;
    jointDef->dampingRatio = 0.0f;
    jointDef->frequencyHz = 4.0f;
    jointDef->Initialize(m_innerBody, hookedBody, m_innerBody->GetWorldCenter(),point);
    B2Helper::Instance()->putJointPool(jointDef);

    
}

void RoleObject::setFaceLeft(bool isFaceLeft)
{
    if(m_weapon->isHooked())
        return;
    m_face->setFlipX(!isFaceLeft);
    if(m_faceLeft == !isFaceLeft)
    {
        b2Body *tempBody;
        tempBody = m_leftBody;
        m_leftBody = m_rightBody;
        m_rightBody = tempBody;

        m_faceLeft = isFaceLeft;
        m_weapon->setIsFaceLeft(isFaceLeft);
        
     
    }
}

void RoleObject::beenAttacked(Monster *monster)
{
    if(isProtected() == false)
    {
        runChangeFaceAction("face_pain.png", 2.0f);
        monster->attacked(m_model);
        //this->scheduleOnce(schedule_selector(RoleObject::setUnProtected), 1.0f);
        CCActionInterval * blink = CCBlink ::create(1, 20);
        CCCallFunc *unProtectedAction = CCCallFunc::create(this, callfunc_selector(RoleObject::setUnProtected));
        CCFiniteTimeAction *seq = CCSequence::create(blink, unProtectedAction, NULL);
        this->runAction(seq);
    }
}

//if weapon attacking area is collied with other body, this function will be called.
void RoleObject::onAttacking(b2Contact *contact, b2Body* otherBody)
{
    CCNode *otherNode = static_cast<CCNode*>(otherBody->GetUserData());
    if(otherNode == NULL)
        return;
    
    m_weapon->interationWithOther(contact, otherBody, m_onTouchDown);

    m_isFlying = true;

    
}
bool RoleObject::isProtected()
{
    return m_isProtected;
}
void RoleObject::setUnProtected()
{
    this->setVisible(true);
    m_isProtected = false;
}
bool RoleObject::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_onTouchDown = true;
    //this->jump(5.0f);
    this->attack();
    return true;
}

bool RoleObject::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if(m_onTouchDown)
    {
        if(m_weapon->isHooked())
        {
            m_weapon->endHook();
        }
    }

    m_onTouchDown = false;
    return true;
}

bool RoleObject::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void RoleObject::setLinearVecByAcceleration(CCAcceleration* pAccelerationValue)
{
    CCLog("set linear ");
    if(m_weapon->isHooked())
        return;
    if(m_isFlying)
    {
        
        //m_innerBody->SetLinearVelocity()
        return;
    }

    float x = pAccelerationValue->x;
    //TODO the maxVecx parm can be affected by this model's agility.
    float maxVecX = 30.0f;

    if(x > -0.1f && x < 0.1f)
        x = 0.0f;

    if(x > 0)
    {
        this->setFaceLeft(false);
    }
    else if (x < 0)
    {
        this->setFaceLeft(true);
    }

    m_innerBody->SetLinearVelocity(b2Vec2(x * maxVecX, m_innerBody->GetLinearVelocity().y));
    CCLog("Role x Velocity : %f", x * maxVecX);
}

void RoleObject::changeFace(CCNode *node, void* pFile)
{
    m_face->initWithFile((char*)pFile);
    m_face->setScale(m_length * 2 / m_face->getContentSize().width);
}

void RoleObject::runChangeFaceAction(char* pFile, float dt)
{
    m_face->stopAllActions();

    CCCallFuncND *face1Func = CCCallFuncND::create(this, callfuncND_selector(RoleObject::changeFace), pFile);
    face1Func->setDuration(dt);
    char *filedir = "face_smile.png";
    CCCallFuncND *face1Normal = CCCallFuncND::create(this, callfuncND_selector(RoleObject::changeFace), filedir);
    
    CCFiniteTimeAction *seq = CCSequence::create(face1Func, face1Normal, NULL); 
    this->runAction(seq);

}

void RoleObject::setWorldPosition(CCPoint position)
{
    if(m_faceLeft)
    {
        m_topBoby->SetTransform(b2Vec2(position.x/PTM_RATIO, (position.y + m_outRadius)/PTM_RATIO), 0.0f);
        m_bottomBody->SetTransform(b2Vec2(position.x/PTM_RATIO, (position.y - m_outRadius)/PTM_RATIO), 0.0f);
        m_leftBody->SetTransform(b2Vec2((position.x - m_outRadius)/PTM_RATIO, position.y/PTM_RATIO), 0.0f);
        m_rightBody->SetTransform(b2Vec2((position.x + m_outRadius)/PTM_RATIO, position.y/PTM_RATIO), 0.0f);
        m_innerBody->SetTransform(b2Vec2(position.x/PTM_RATIO, position.y/PTM_RATIO), 0.0f);
    }
    else
    {
        m_topBoby->SetTransform(b2Vec2(position.x/PTM_RATIO, (position.y + m_outRadius)/PTM_RATIO), 0.0f);
        m_bottomBody->SetTransform(b2Vec2(position.x/PTM_RATIO, (position.y - m_outRadius)/PTM_RATIO), 0.0f);
        m_rightBody->SetTransform(b2Vec2((position.x - m_outRadius)/PTM_RATIO, position.y/PTM_RATIO), 0.0f);
        m_leftBody->SetTransform(b2Vec2((position.x + m_outRadius)/PTM_RATIO, position.y/PTM_RATIO), 0.0f);
        m_innerBody->SetTransform(b2Vec2(position.x/PTM_RATIO, position.y/PTM_RATIO), 0.0f);
    }


    setB2NodePostion(position);
}

void RoleObject::resetRole()
{
    setWorldPosition(ccp(CCDirector::sharedDirector()->getVisibleSize().width/2, 200));

    update(1);
}

bool RoleObject::passingBorder(int xPosition)
{
    CCLog("Role xPosition : %d", xPosition);
    int safeOffset = 10;
    CCSize size = CCDirector::sharedDirector()->getVisibleSize();
    if(xPosition < -m_length - safeOffset)
    {
        CCPoint position = ccp(m_innerBody->GetPosition().x * PTM_RATIO, m_innerBody->GetPosition().y * PTM_RATIO);
        this->setWorldPosition(ccp(size.width + m_length, position.y));

        return true;
    }
    else if(xPosition >size.width + m_length + safeOffset)
    {
        CCPoint position = ccp(m_innerBody->GetPosition().x * PTM_RATIO, m_innerBody->GetPosition().y * PTM_RATIO);

        this->setWorldPosition(ccp(-m_length, position.y));

        return true;
    }

    return false;
}

void RoleObject::tryLaunchParticle(float dt)
{
    CCParticleSystemQuad *particle = CCParticleSystemQuad::create("particle1.plist");
    particle->setAnchorPoint(ccp(0.5f, 0.5));
    particle->setPosition(ccp(m_bottomBody->GetPosition().x * PTM_RATIO, m_bottomBody->GetPosition().y * PTM_RATIO));
    particle->setAutoRemoveOnFinish(true);
    this->addChild(particle);

}

void RoleObject::setAcceleration(CCAcceleration *acceleration)
{
    m_acceleration = *acceleration;
}

void RoleObject::updateAcceleration(float dt)
{
    CCLog("update accleration");
    if(m_weapon->isHooked())
        return;

    b2Vec2 speed = m_innerBody->GetLinearVelocity();
    float x ;
    x = m_acceleration.x;
        
 
    //TODO the maxVecx parm can be affected by this model's agility.
    float maxVecX = 30.0f;

    if(x > -0.1f && x < 0.1f)
        x = 0.0f;

    if(x > 0)
    {
        this->setFaceLeft(false);
    }
    else if (x < 0)
    {
        this->setFaceLeft(true);
    }

    float speedX = speed.x;
    if(abs(speed.x) <= 0.5)
        speed.x = 0.0f;
    CCLog("x: %f, speed.x:%f", x, speed.x);
    if(x * speed.x < 0)
    {
        /*speedX = speedX + x * 60;
        speedX = clampf(speedX, -30.0f, 30.0f);*/
        m_innerBody->SetLinearVelocity(b2Vec2(0.0f, speed.y));
    }
    else
    {
        speedX = speedX + x * 20;
        speedX = clampf(speedX, -30.0f, 30.0f);
        m_innerBody->SetLinearVelocity(b2Vec2(speedX, speed.y));
    }

   
    //m_innerBody->SetLinearVelocity(b2Vec2(speedX, speed.y));
}

void RoleObject::setColliedFlag()
{
    m_blockColliedFlag = false;
}