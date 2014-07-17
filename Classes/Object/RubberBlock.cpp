#include "RubberBlock.h"

RubberBlock::RubberBlock()
{
	m_polyBody = NULL;
	m_texture = NULL;
}

RubberBlock::~RubberBlock()
{
    B2Helper::Instance()->putDeadPool(m_topBody);
    B2Helper::Instance()->putDeadPool(m_bottomBody);
}

RubberBlock* RubberBlock::createRubberBlock(CCPoint point, CCSize size, void *parm)
{
	RubberBlock *pRet = new RubberBlock();
	if(pRet && pRet->initWithPointSize(point, size, parm))
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

bool RubberBlock::initWithPointSize(CCPoint point, CCSize size, void *parm)
{
    //Add code for config support.
    //now we get the size and other parm from (void* parm) the xml_node<> *node
    xml_node<>* node = (xml_node<> *)parm;
    m_size = CCSizeMake( atoi(node->first_attribute("Width")->value()), atoi(node->first_attribute("Height")->value()));
    m_position = point;

    m_texture = cocos2d::CCTextureCache::sharedTextureCache()->addImage(node->first_attribute("Texture")->value());
	/*m_position = point;
	m_size	= size;*/
	//this->setTag(TagHelper::Instance()->getBlockTag());

	createBody();
	initRenderData();
	
	setAlive(false);

	return true;
}

bool RubberBlock::initWithConfigNode(xml_node<> *node)
{
    m_size = CCSizeMake( atoi(node->first_attribute("Width")->value()), atoi(node->first_attribute("Height")->value()));
    m_texture = cocos2d::CCTextureCache::sharedTextureCache()->addImage(node->first_attribute("Texture")->value());
    m_position = ccp(0, 0);
    
    createBody();
    initRenderData();

    setAlive(false);

    //this->scheduleUpdate();
    return true;
}

void RubberBlock::setAlive(bool flag)
{
	if(flag)
	{	
		m_isAlive = true;
        B2Helper::Instance()->putActivePool(m_topBody);
        B2Helper::Instance()->putActivePool(m_bottomBody);
	}
	else
	{
		m_isAlive = false;
		B2Helper::Instance()->putSleepingPool(m_topBody);
        B2Helper::Instance()->putSleepingPool(m_bottomBody);
	}   

}

bool RubberBlock::setBlockPosition(cocos2d::CCPoint position)
{
    setAlive(true);

	m_position = position;

    m_bottomBody->SetTransform(b2Vec2(m_position.x/PTM_RATIO, m_position.y/PTM_RATIO - polyHeight * 1.5f), 0.0f);
    m_topBody->SetTransform(b2Vec2(m_position.x/PTM_RATIO, m_position.y/PTM_RATIO + polyHeight * 1.5f), 0.0f);

    return true;
}

void RubberBlock::onB2PositionChanged()
{
    setAlive(true);
    this->setPosition(m_position);
    m_bottomBody->SetTransform(b2Vec2(m_position.x/PTM_RATIO, m_position.y/PTM_RATIO - polyHeight * 1.5f), 0.0f);
    m_topBody->SetTransform(b2Vec2(m_position.x/PTM_RATIO, m_position.y/PTM_RATIO + polyHeight * 1.5f), 0.0f);
    //tryLaunchParticle(m_position);
}

void RubberBlock::interationWithOther(b2Body* otherBody)
{
	CCNode *otherNode =(CCNode*) (otherBody->GetUserData());
	CCLog("Interation with %d", otherNode->getTag());
}

bool RubberBlock::createBody()
{
    b2PolygonShape polyShape;

    polyWidth = m_size.width / PTM_RATIO;
    polyHeight = (m_size.height / 4) / PTM_RATIO;

    polyShape.SetAsBox(polyWidth/2, polyHeight / 2);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &polyShape;
    fixtureDef.friction = 0.5;
    fixtureDef.restitution = 1;
    fixtureDef.filter.categoryBits = BM_BLOCK;
    fixtureDef.filter.maskBits =  BM_WEAPON | BM_BLOCK;
    //fixtureDef.filter.groupIndex = 2;

    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position = b2Vec2(m_position.x/PTM_RATIO, m_position.y/PTM_RATIO - polyHeight * 1.5f);
    bodyDef.fixedRotation = false;
    //create the bottom body;
    m_bottomBody = B2Helper::Instance()->getWorld()->CreateBody(&bodyDef);
    m_bottomBody->CreateFixture(&fixtureDef);
    m_bottomBody->SetUserData(this);

    bodyDef.position = b2Vec2(m_position.x/PTM_RATIO, m_position.y/PTM_RATIO + polyHeight * 1.5f);
    bodyDef.type = b2_dynamicBody;
    bodyDef.fixedRotation = false;
    
    fixtureDef.filter.maskBits = BM_ROLE | BM_BLOCK;

    m_topBody = B2Helper::Instance()->getWorld()->CreateBody(&bodyDef);
    m_topBody->CreateFixture(&fixtureDef);
    m_topBody->SetUserData(this);
    //m_topBody->ApplyAngularImpulse(10.0f);

    b2Vec2 jointOffset;
    jointOffset = b2Vec2(m_size.width/PTM_RATIO/2, 0);
    b2DistanceJointDef jointDef;
    jointDef.collideConnected = true;
    jointDef.dampingRatio = 0.1f;
    jointDef.frequencyHz = 5.0f;
    jointDef.Initialize(m_topBody, m_bottomBody, m_topBody->GetWorldCenter() - jointOffset, m_bottomBody->GetWorldCenter() - jointOffset);
    B2Helper::Instance()->getWorld()->CreateJoint(&jointDef);

    jointDef.Initialize(m_topBody, m_bottomBody, m_topBody->GetWorldCenter() + jointOffset, m_bottomBody->GetWorldCenter() + jointOffset);
    B2Helper::Instance()->getWorld()->CreateJoint(&jointDef);

    b2PrismaticJointDef prisJointDef;
    prisJointDef.enableLimit = false;
    prisJointDef.enableMotor = true;
    prisJointDef.collideConnected = true;
    prisJointDef.upperTranslation = 1.0f;
    prisJointDef.lowerTranslation = 1.0f;
    prisJointDef.Initialize(m_bottomBody, m_topBody, m_bottomBody->GetWorldCenter(), b2Vec2(0.0f, 1.0f));
    B2Helper::Instance()->getWorld()->CreateJoint(&prisJointDef);

    return true;
}

bool RubberBlock::initRenderData()
{
	m_textCoord[0] = Vertex2DMake(0.0f, 0.0f);
	m_textCoord[1] = Vertex2DMake(1.0f, 0.0f);
	m_textCoord[2] = Vertex2DMake(0.0f, 1.0f);
	m_textCoord[3] = Vertex2DMake(1.0f, 1.0f);

	return true;
}

void RubberBlock::draw()
{   
	if(!m_isAlive)
		return;

    ccGLBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   
    m_vertexCoord[0] = Vertex2DMake(-m_size.width/2, m_topBody->GetPosition().y*PTM_RATIO - m_position.y + m_size.height/4);
    m_vertexCoord[1] = Vertex2DMake(m_size.width/2 , m_topBody->GetPosition().y*PTM_RATIO - m_position.y + m_size.height/4);
    m_vertexCoord[2] = Vertex2DMake(-m_size.width/2,  m_bottomBody->GetPosition().y*PTM_RATIO - m_position.y - m_size.height/4);
    m_vertexCoord[3] = Vertex2DMake(m_size.width/2, m_bottomBody->GetPosition().y*PTM_RATIO - m_position.y - m_size.height/4);

 /*   m_vertexCoord[0] = Vertex2DMake( -m_size.width/2, m_size.height/2);
    m_vertexCoord[1] = Vertex2DMake( m_size.width/2, m_size.height/2);
    m_vertexCoord[2] = Vertex2DMake( -m_size.width/2, -m_size.height/2);
    m_vertexCoord[3] = Vertex2DMake( m_size.width/2, -m_size.height/2);*/

	ccGLBindTexture2D(m_texture->getName());
	m_texture->getShaderProgram()->use();
	m_texture->getShaderProgram()->setUniformsForBuiltins();
	ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords);
	glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, m_textCoord);
	glVertexAttribPointer(kCCVertexAttrib_Position,  2, GL_FLOAT, GL_FALSE, 0, m_vertexCoord);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void RubberBlock::onCollied(b2Contact *contact, b2Body *bodyOther)
{
    //tryLaunchParticle();
 //   if(!contact->IsEnabled())
 //       return;
	////this->setAlive(false);
 //   B2CCNode *node = (B2CCNode*)(bodyOther->GetUserData());

 //   if(TagHelper::Instance()->isObject(node->getTag(), ON_ROLE))
 //   {
 //      
 //       RoleObject *role = (RoleObject*)(bodyOther->GetUserData());

 //       role->jump(20.0f);
 //   }
    if(bodyOther->GetUserData() == this)
    {
        CCLog("bottom block collied with top");
    }
}

bool RubberBlock::tryLaunchParticle(CCPoint emitPoint)
{

    CCParticleSystemQuad *particle = CCParticleSystemQuad::create("NewBlockParticle_1.plist");
    particle->setAnchorPoint(ccp(0.5f, 0.5));
    particle->setPosition(m_position);
    particle->setAutoRemoveOnFinish(true);
    
    
    this->addChild(particle);

    return true;
}

void RubberBlock::update(float dt)
{
    m_time += dt;
}

RubberBlock* RubberBlock::createWithConfigNode(xml_node<> *node)
{
    RubberBlock *pRet = new RubberBlock();
    if(pRet && pRet->initWithConfigNode(node))
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