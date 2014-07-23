#include "MoveBlock.h"

MoveBlock::MoveBlock()
{

}

MoveBlock::~MoveBlock()
{

}

MoveBlock* MoveBlock::createMoveBlock(xml_node<> *node, float timeCircle, float speed)
{
    MoveBlock *pRet = new MoveBlock();

    if(pRet && pRet->initMoveBLock(node, timeCircle, speed))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return pRet;
    }
}

bool MoveBlock::initMoveBLock(xml_node<> *node, float timeCircle, float speed)
{
    CCLog("3debug here!!");

    RubberBlock::initWithConfigNode(node);
    //this->setColor(ccRED);
    //this->changeTexture("scoreboard.png");

    m_time = 0.0f;
    m_timeCircle = CCRANDOM_0_1() * 2 + 3;
    m_speed = CCRANDOM_0_1() * 100 + 120;
    m_stopMove = false;

    //this->schedule(schedule_selector(MoveBlock::updateMove), 0.1);

    return true;
}

void MoveBlock::updateMove(float dt)
{
    if(!m_isAlive || m_stopMove)
        return;
    m_time += dt;


    float halfCircle = m_timeCircle / 2.0f;

    int flag = 1;

    if(fmod(m_time, m_timeCircle) < halfCircle)
        flag = 1;
    else
        flag = -1;

    CCPoint position = this->getB2NodePostion();
    this->setB2NodePostion(ccp(position.x + flag * dt * m_speed, position.y));
}

void MoveBlock::onCollied(b2Contact *contact, b2Body *bodyOther)
{

    
}

void MoveBlock::setAlive(bool flag)
{
    RubberBlock::setAlive(flag);
    m_stopMove = false;
    this->schedule(schedule_selector(MoveBlock::updateMove));


}

void MoveBlock::colliedWithRole(b2Contact *contact, b2Body *bodyOther)
{
    m_stopMove = true;
}