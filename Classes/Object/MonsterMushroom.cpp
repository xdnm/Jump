#include "Object/MonsterMushroom.h"

MonsterMushroom::MonsterMushroom()
{
    m_topBody = NULL;
    m_bottomBody = NULL;
    m_position = ccp(0, 0);
    m_size = CCSizeMake(0, 0);
    m_texture = NULL;

    this->setTag(TagHelper::Instance()->getTag(ON_MONSTOR));
}

MonsterMushroom::~MonsterMushroom()
{
    if(m_topBody != NULL)
        B2Helper::Instance()->putDeadPool(m_topBody);

    if(m_bottomBody != NULL)
        B2Helper::Instance()->putDeadPool(m_bottomBody);

}

MonsterMushroom* MonsterMushroom::createMushroom(CCLayer *layer, CCPoint position, CCSize size, void *parm)
{
    MonsterMushroom *pRet = new MonsterMushroom();

    if(pRet != NULL && pRet->initMushroom(layer, position, size, parm))
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

bool MonsterMushroom::initMushroom(CCLayer *layer, CCPoint position, CCSize size, void *parm)
{
    m_layer = layer;
    m_position = position;
    m_size = size;
    this->setPosition(position);

    m_texture = cocos2d::CCTextureCache::sharedTextureCache()->addImage("mushroom.png");
    m_textureCoords[0] = Vertex2DMake(0, 0);
    m_textureCoords[1] = Vertex2DMake(1, 0);
    m_textureCoords[2] = Vertex2DMake(0, 1);
    m_textureCoords[3] = Vertex2DMake(1, 1);


    initModel();

    createBody();
    return true;
}

bool MonsterMushroom::initModel()
{
    if(m_model == NULL)
        m_model = new ObjectModel();
    m_model->m_damege = 10;
    m_model->m_defence = 10;
    m_model->m_health = 51;

    this->m_mainBody = m_topBody;
    this->m_visiableNode = this;

    m_model->m_B2Node = this;
    return true;
}
bool MonsterMushroom::createBody()
{
    b2PolygonShape polyShape;
    float polyWidth, polyHeight;
    polyWidth = m_size.width / PTM_RATIO;
    polyHeight = (m_size.height / 4) / PTM_RATIO;

    polyShape.SetAsBox(polyWidth/2, polyHeight / 2);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &polyShape;
    fixtureDef.friction = 0;
    fixtureDef.restitution = 1;
    fixtureDef.filter.categoryBits = BM_MONSTOR;
    fixtureDef.filter.maskBits = BM_ROLE | BM_WEAPON | BM_MONSTOR;

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
     m_topBody = B2Helper::Instance()->getWorld()->CreateBody(&bodyDef);
     m_topBody->CreateFixture(&fixtureDef);
     m_topBody->SetUserData(this);
     m_topBody->ApplyAngularImpulse(10.0f);

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
     prisJointDef.upperTranslation = 1.0f;
     prisJointDef.lowerTranslation = 1.0f;
     prisJointDef.Initialize(m_bottomBody, m_topBody, m_bottomBody->GetWorldCenter(), b2Vec2(0.0f, 1.0f));
     B2Helper::Instance()->getWorld()->CreateJoint(&prisJointDef);

     return true;
}

void MonsterMushroom::draw()
{


    m_vertexCoords[0] = Vertex2DMake(-m_size.width/2, m_topBody->GetPosition().y*PTM_RATIO - m_position.y + m_size.height/4);
    m_vertexCoords[1] = Vertex2DMake(m_size.width/2 , m_topBody->GetPosition().y*PTM_RATIO - m_position.y + m_size.height/4);
    m_vertexCoords[2] = Vertex2DMake(-m_size.width/2,  m_bottomBody->GetPosition().y*PTM_RATIO - m_position.y - m_size.height/4);
    m_vertexCoords[3] = Vertex2DMake(m_size.width/2, m_bottomBody->GetPosition().y*PTM_RATIO - m_position.y - m_size.height/4);

    ccGLBindTexture2D(m_texture->getName());
    m_texture->getShaderProgram()->use();
    m_texture->getShaderProgram()->setUniformsForBuiltins();
    ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, m_textureCoords);
    glVertexAttribPointer(kCCVertexAttrib_Position,  2, GL_FLOAT, GL_FALSE, 0, m_vertexCoords);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

bool MonsterMushroom::isReady()
{
    if(m_topBody->GetLinearVelocity().Length() > 1.0f)
        return false;
    else
        return true; 
}

bool MonsterMushroom::attacked(ObjectModel *model)
{
    CCLOG("tag %d attacked role!", this->getTag());
    //model->m_B2Node->m_mainBody->ApplyForce(b2Vec2(100.0f, 0), model->m_B2Node->m_mainBody));

    B2CCNode *node = model->m_B2Node;
    float force = 4.0f;
    if(node->getB2NodePostion().x > m_position.x)
        node->m_mainBody->ApplyLinearImpulse(b2Vec2(force, force/2), node->m_mainBody->GetWorldCenter());
    else
        node->m_mainBody->ApplyLinearImpulse(b2Vec2(-force, force/2), node->m_mainBody->GetWorldCenter());

    model->beenAttackWithModel(m_model);
    return true;
}

bool MonsterMushroom::beenTrampled(ObjectModel *model)
{
    CCLOG("tag : %d been trampled", this->getTag());
    this->m_model->beenAttackWithModel(model);
    this->checkHealth();

    return true;
}

bool MonsterMushroom::beenAttacked(ObjectModel *model)
{
    CCLOG("tag %d been attaced!", this->getTag());
    this->m_model->beenAttackWithModel(model);
    this->checkHealth();
    return true;
}