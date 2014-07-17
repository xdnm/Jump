#include "ScoreBoard.h"

ScoreBoard::ScoreBoard()
{
    
}

ScoreBoard::~ScoreBoard()
{

}

ScoreBoard* ScoreBoard::createScoreBoard(CCNode* role)
{
    ScoreBoard* pRet = new ScoreBoard();

    if(pRet && pRet->initWithRole(role))
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

bool ScoreBoard::initWithRole(CCNode *layer)
{
    this->setAnchorPoint(ccp(0.5f, 0.5f));

    CCSize size = CCDirector::sharedDirector()->getVisibleSize();
    m_layer = layer;
    m_label = CCLabelAtlas::create("0", "fonts/Text2.plist");
    m_label->setAnchorPoint(ccp(0.5f, 0.5f));
    m_label->setScale((size.height/18) /m_label->getContentSize().height);
    
    m_label->setColor(ccRED);

    this->addChild(m_label);
    reset();
    this->schedule(schedule_selector(ScoreBoard::scoreUpdate));
    return true;
}

void ScoreBoard::setScore(int height)
{
    CCSize size = CCDirector::sharedDirector()->getVisibleSize();
    char s[20];
    if(height < 0)
        height = -height;
    sprintf(s, "%d", height);
    m_label->setString(s);

    this->setPosition(ccp(size.width/2, size.height-60));
}

void ScoreBoard::scoreUpdate(float dt)
{
    m_sumScore = (-m_layer->getPositionY()/m_scoreRate) + m_bonusSumScore;
    setScore(m_sumScore);
}

void ScoreBoard::reset()
{
    m_lastBonus = 100;
    m_scoreRate = 20;
    m_sumScore = 0;
    m_bonusSumScore = 0;
}

void ScoreBoard::addBonus(int bouns /* = 0 */)
{
    if(bouns == 0)
    {
        m_lastBonus = m_lastBonus * 2.0f;
        m_bonusSumScore += m_lastBonus;
    }
    else
    {
        //m_lastBonus += bouns;
        m_bonusSumScore += bouns;
    }
}

void ScoreBoard::clearBonus()
{
    m_lastBonus = 100;
}

