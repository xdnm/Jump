#include "RigidBlock.h"
#include "Utils/GlobalData.h"

RigidBlock::RigidBlock()
{
	m_polyBody = NULL;
	m_texture = NULL;
}

RigidBlock::~RigidBlock()
{
    B2Helper::Instance()->putDeadPool(m_polyBody);

}

RigidBlock* RigidBlock::createRigidBlock(CCPoint point, CCSize size, void *parm)
{
	RigidBlock *pRet = new RigidBlock();
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

bool RigidBlock::initWithPointSize(CCPoint point, CCSize size, void *parm)
{
    
	m_position = point;
	m_size	= size;
	//this->setTag(TagHelper::Instance()->getBlockTag());

	createBody();
	initRenderData();
	
	setAlive(false);

	return true;
}

void RigidBlock::setAlive(bool flag)
{
	if(flag)
	{	
		m_isAlive = true;
        B2Helper::Instance()->putActivePool(m_polyBody);
	}
	else
	{
		m_isAlive = false;
		B2Helper::Instance()->putSleepingPool(m_polyBody);
	}   

}

bool RigidBlock::setBlockPosition(cocos2d::CCPoint position)
{
    setAlive(true);

	m_position = position;

	m_polyBody->SetTransform(b2Vec2(m_position.x/PTM_RATIO, m_position.y/PTM_RATIO), 0.0f);

    return true;
}

void RigidBlock::onB2PositionChanged()
{
    setAlive(true);

    m_polyBody->SetTransform(b2Vec2(m_position.x/PTM_RATIO, m_position.y/PTM_RATIO), 0.0f);

}

void RigidBlock::interationWithOther(b2Body* otherBody)
{
	CCNode *otherNode =(CCNode*) (otherBody->GetUserData());
	CCLog("Interation with %d", otherNode->getTag());
}

bool RigidBlock::createBody()
{
	b2PolygonShape polyShape;
	polyShape.SetAsBox(m_size.width / PTM_RATIO /2, m_size.height / PTM_RATIO / 2);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &polyShape;
	fixtureDef.density = 10.0f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.friction = 1.0f;
    fixtureDef.filter.categoryBits = BM_BLOCK;
    fixtureDef.filter.maskBits = BM_ROLE | BM_WEAPON;

    /*fixtureDef.filter.groupIndex = -10;*/

	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position = b2Vec2(m_position.x/PTM_RATIO, m_position.y	/PTM_RATIO);
	bodyDef.fixedRotation = true;

	m_polyBody = B2Helper::Instance()->getWorld()->CreateBody(&bodyDef);
	m_polyBody->CreateFixture(&fixtureDef);

	m_polyBody->SetUserData(this);
	return true;
}

bool RigidBlock::initRenderData()
{
	m_texture = cocos2d::CCTextureCache::sharedTextureCache()->addImage("rigidblock.png");
	
	m_textCoord[0] = Vertex2DMake(0.0f, 0.0f);
	m_textCoord[1] = Vertex2DMake(1.0f, 0.0f);
	m_textCoord[2] = Vertex2DMake(0.0f, 1.0f);
	m_textCoord[3] = Vertex2DMake(1.0f, 1.0f);

	return true;
}

void RigidBlock::draw()
{
	if(!m_isAlive)
		return ;

    m_vertexCoord[0] = Vertex2DMake((m_position.x - m_size.width/2), (m_position.y + m_size.height/2) );
    m_vertexCoord[1] = Vertex2DMake((m_position.x + m_size.width/2), (m_position.y + m_size.height/2));
    m_vertexCoord[2] = Vertex2DMake((m_position.x - m_size.width/2), (m_position.y - m_size.height/2) );
    m_vertexCoord[3] = Vertex2DMake((m_position.x + m_size.width/2), (m_position.y - m_size.height/2));


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

void RigidBlock::onCollied(b2Contact *contact, b2Body *bodyOther)
{

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
}

