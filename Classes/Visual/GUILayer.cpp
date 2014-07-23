#include "Visual/GUILayer.h"

GUILayer* GUILayer::instance = NULL;

GUILayer::GUILayer()
{

}

GUILayer::~GUILayer()
{

}

GUILayer* GUILayer::createGUILayerWithScene(CCNode *scene, CCLayer *layer)
{
    instance = new GUILayer();

    if(instance && instance->initWithGameLayerWithScene(scene, layer))
    {
        instance->autorelease();
        return instance;
    }
    else
    {
        delete instance;
        instance = NULL;
        return NULL;
    }
}

bool GUILayer::initWithGameLayerWithScene(CCNode *scene, CCLayer *layer)
{
    m_gameLayer = layer;

    m_scoreBoard = ScoreBoard::createScoreBoard(layer);
    this->addChild(m_scoreBoard, 10);

    m_hookCount = 0;
    m_powerUnit = 2;

    m_powerColum = PowerColumn::createPowerColumn();
    m_powerColum->reset();
    m_powerColum->setPosition(10, 10);

    this->addChild(m_powerColum);
    

    scene->addChild(this);

    return true;
}

GUILayer* GUILayer::createGUILayer(CCLayer *gameLayer)
{
    GUILayer* pRet = new GUILayer();

    if(pRet && pRet->initWithGameLayer(gameLayer))
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

bool GUILayer::initWithGameLayer(CCLayer *gameLayer)
{
    m_gameLayer = gameLayer;
    
    m_scoreBoard = ScoreBoard::createScoreBoard(gameLayer);

    
    return true;
}

void GUILayer::runDeadBoard()
{
    m_deadEndBoard = DeadEndBoard::createDeadEndBoard(m_scoreBoard->m_sumScore);
    this->addChild(m_deadEndBoard, 10);
}

void GUILayer::stopDeadBoard()
{
    this->removeChild(m_deadEndBoard);
    m_deadEndBoard = NULL;
}

bool GUILayer::isDeadBoard()
{
    return m_deadEndBoard != NULL;
}

void GUILayer::reset()
{
    stopDeadBoard();
    m_scoreBoard->reset();
    m_powerColum->reset();

    m_powerUnit = 2;
}

GUILayer * GUILayer::Instance()
{
    return instance;
}

void GUILayer::onRoleJumping()
{
    m_hookCount = 0;
    m_scoreBoard->clearBonus();
    m_powerColum->addPower(m_powerUnit);
}

void GUILayer::onRoleHooking()
{
    runBounsAction();
    m_hookCount++;
    if(m_hookCount > 7)
        m_hookCount = 7;
    if(m_hookCount >= 2)
    {

        char *pFile;
        float scaleRate = 1.0f;
        switch (m_hookCount)
        {
        case 2:
            pFile = "good.png";
            scaleRate = 1.2f;
            break;
        case 3:
            pFile = "great.png";
            scaleRate = 1.3f;
            break;
        case 4:
            pFile = "nice.png";
            scaleRate = 1.4f;
            break;
        case 5:
            pFile = "cool.png";
            scaleRate = 1.5f;
            break;
        case 6:
            pFile = "perfect.png";
            scaleRate = 1.6f;
        default:
            pFile = "perfect.png";
            scaleRate = 1.6f;
            break;
        }

        CCSize size = CCDirector::sharedDirector()->getVisibleSize();

        CCSprite *textSprite = CCSprite::create(pFile);
        textSprite->setScale(40.0/textSprite->getContentSize().height);
        textSprite->setAnchorPoint(ccp(0.5f, 0.5f));
        textSprite->setPosition(ccp(size.width/2, size.height/2));
        textSprite->setColor(ccRED);

        CCActionInterval * scaleto = CCScaleBy ::create(0.2f, scaleRate);
        CCActionInterval *scaleback = CCScaleBy::create(0.2f, 0.8f);
        CCCallFuncN *callfunc = CCCallFuncN::create(this, callfuncN_selector(GUILayer::textAnimationEnd));

        CCFiniteTimeAction *seq = CCSequence::create(scaleto, scaleback, callfunc, NULL);
        textSprite->runAction(seq);

        this->addChild(textSprite);
    }

 

    m_scoreBoard->addBonus();
    m_powerColum->addPower(10);
}

int GUILayer::getRolePower()
{
    return m_powerColum->getPower();
}

void GUILayer::clearRolePower()
{
    m_powerColum->reset();
}

void GUILayer::textAnimationEnd(CCNode *node)
{
    node->removeFromParent();
}

void GUILayer::addBouns(int bouns)
{
    m_scoreBoard->addBonus(bouns);
    runBounsAction();
}

void GUILayer::runBounsAction()
{
    CCActionInterval *scaleAction = CCScaleBy::create(0.1f, 1.3f, 1.2f);
    m_scoreBoard->runAction(CCSequence::create(scaleAction, scaleAction->reverse(), NULL));
}

void GUILayer::decreaseScoreRatio()
{
    //m_scoreBoard->m_scoreRate--;
    if(m_scoreBoard->m_scoreRate <= 10)
        m_scoreBoard->m_scoreRate = 10;
    else
        m_scoreBoard->m_scoreRate--;
}

void GUILayer::addPowerUnit(int parm /* = 0 */)
{
    if(parm == 0)
    {
        m_powerUnit += 2;
    }
    else
    {
        m_powerUnit += parm;
    }
}