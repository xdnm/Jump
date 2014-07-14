#include "PowerColumn.h"

PowerColumn::PowerColumn()
{

}

PowerColumn::~PowerColumn()
{

}

PowerColumn* PowerColumn::createPowerColumn()
{
    PowerColumn *pRet = new PowerColumn();

    if(pRet && pRet->initPowerColumn())
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

bool PowerColumn::initPowerColumn()
{
    CCNode::init();
    //this->setAnchorPoint(ccp(0.5f, 0.5f));
    
    this->setContentSize(CCSizeMake(50, 200));
    
    m_texture = CCTextureCache::sharedTextureCache()->addImage("color.png");
    m_time = 0.0f;  
    m_power = 0.0f;
    m_maxPower = 100.0f;

    this->scheduleUpdate();

    CCSprite *bott = CCSprite::create("colum.png");
    bott->setAnchorPoint(ccp(0.0f, 0.0f));
    bott->setScaleX(this->getContentSize().width/ bott->getContentSize().width);
    bott->setScaleY(this->getContentSize().height / bott->getContentSize().height);
    //bott->setOpacity(0.1f);
    this->addChild(bott);

   /* CCSprite *temp = CCSprite::create("mushroom.png");
    this->addChild(temp, 1);*/
    return true;
}

void PowerColumn::update(float delta)
{
    m_time += delta;
}

void PowerColumn::draw()
{
    //float offset = 
    CCSize size = this->getContentSize();
    float rate = m_power/m_maxPower;
    float height = size.height * rate;
    Vertex2D vertexCoord[4];
    vertexCoord[0] = Vertex2DMake(2.0f, 2.0f);
    vertexCoord[1] = Vertex2DMake(size.width - 2.0f, 2.0f);
    vertexCoord[2] = Vertex2DMake(2.0f, height -2.0f);
    vertexCoord[3] = Vertex2DMake(size.width-2.0f, height-2.0f);

    Vertex2D vertexCoord1[4];
    vertexCoord1[0] = Vertex2DMake(0.0f, height);
    vertexCoord1[1] = Vertex2DMake(size.width, height);
    vertexCoord1[2] = Vertex2DMake(size.width, 0);
    vertexCoord1[3] = Vertex2DMake(0.0f, height);

    Vertex2D texCoords[4];
    texCoords[0] = Vertex2DMake(0.0f, 0.0f);
    texCoords[1] = Vertex2DMake(1.0f, 0.0f);
    texCoords[2] = Vertex2DMake(0.0f, rate);
    texCoords[3] = Vertex2DMake(1.0f, rate);

    ccGLBindTexture2D(m_texture->getName());
    m_texture->getShaderProgram()->use();
    m_texture->getShaderProgram()->setUniformsForBuiltins();
    ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, texCoords);
    glVertexAttribPointer(kCCVertexAttrib_Position,  2, GL_FLOAT, GL_FALSE, 0, vertexCoord);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void PowerColumn::addPower(float power)
{
    m_power += power;
    if(m_power > 100)
        m_power = 100;
}

void PowerColumn::reset()
{
    m_power = 0;
}

int PowerColumn::getPower()
{
    return m_power;
}