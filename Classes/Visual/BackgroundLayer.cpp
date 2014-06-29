#include "BackgroundLayer.h"

BackgroundLayer::BackgroundLayer()
{

}

BackgroundLayer::~BackgroundLayer()
{

}

BackgroundLayer* BackgroundLayer::createBackground(CCScene *scene, CCLayer *watchLayer)
{
    BackgroundLayer *pRet = NULL;
    pRet = new BackgroundLayer();

    if(pRet != NULL && pRet->initBackground(scene, watchLayer))
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

bool BackgroundLayer::initBackground(CCScene *scene, CCLayer *watchLayer)
{
    this->init();
    m_watchLayer = watchLayer;
   
    m_background1 = CCSprite::create("background3.png");
    m_background2 = CCSprite::create("background4.png");

    m_background1->setAnchorPoint(ccp(0, 0));
    m_background2->setAnchorPoint(ccp(0, 0));

    m_background1->setScale(CCDirector::sharedDirector()->getVisibleSize().width / m_background1->getContentSize().width);
    m_background2->setScaleX(CCDirector::sharedDirector()->getVisibleSize().width / m_background2->getContentSize().width);
    m_background2->setScaleY(CCDirector::sharedDirector()->getVisibleSize().height / m_background2->getContentSize().height);

    this->addChild(m_background1, -5000);
    this->addChild(m_background2, -5001);

    scene->addChild(this, -10);

    //CircleLight *light = CircleLight::createCircleLight(80, 100);
    CircleLight *light = CircleLight::createCircleLight(10, 60, 5.0f, 0.4f, ccc4f(1.0f, 1.0f, 0.0f, 0.0f));
    light->setWatchLayer(m_watchLayer, 100);
    light->setRelativePosition(ccp(100, 100));

    CircleLight *light1 = CircleLight::createCircleLight(20, 90, 3.0f, 0.4f, ccc4f(0.8f, 1.0f, 0.0f, 0.0f));
    light1->setWatchLayer(m_watchLayer, 30);
    light1->setRelativePosition(ccp(400, 400));

    this->addChild(light1);
    this->addChild(light);
    this->scheduleUpdate();
    return true;
}

void BackgroundLayer::update(float dt)
{
    height = m_watchLayer->getPositionY();

    int opacity = -height/300;
    if(opacity >= 255)
        opacity = 255;

    m_background1->setOpacity(255 - opacity);
    m_background2->setOpacity(opacity);

    CCLOG("%d", opacity);
}