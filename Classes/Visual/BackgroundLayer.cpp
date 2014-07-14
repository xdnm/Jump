#include "BackgroundLayer.h"

BackgroundLayer::BackgroundLayer()
{

}

BackgroundLayer::~BackgroundLayer()
{

}

BackgroundLayer* BackgroundLayer::createBackground(CCNode *scene, CCLayer *watchLayer)
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

bool BackgroundLayer::initBackground(CCNode *scene, CCLayer *watchLayer)
{
    this->init();
    m_watchLayer = watchLayer;

    m_maxLight = 6;
    m_lightNum = 0;
    m_lastLightHeight = 0;
   
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
    CircleLight *light = CircleLight::createCircleLight(30, 60, 5.0f, 0.1f, ccc4f(1.0f, 0.0f, 0.0f, 0.0f));
    light->setWatchLayer(m_watchLayer, 100);
    light->setRelativePosition(ccp(100, 500));

    CircleLight *light1 = CircleLight::createCircleLight(80, 90, 3.0f, 0.5f, ccc4f(0.8f, 0.0f, 0.0f, 0.0f));
    light1->setWatchLayer(m_watchLayer, 10);
    light1->setRelativePosition(ccp(400, 400));

    this->addChild(light1);
    this->addChild(light);
    this->scheduleUpdate();
    return true;
}

void BackgroundLayer::update(float dt)
{
    height = m_watchLayer->getPositionY();
    int ralativeheight = -height;

    int opacity = -height/300;
    if(opacity >= 255)
        opacity = 255;

    m_background1->setOpacity(255 - opacity);
    m_background2->setOpacity(opacity);

    if(ralativeheight > m_lastLightHeight + 300)
    {
        if(CCRANDOM_0_1() < 0.3f)
        {
            int radius = rand()%100 + 30;
             float distanceRate = 130.0f/radius;
            CircleLight *tempLight = CircleLight::createCircleLight(CCRANDOM_0_1() * radius, radius, distanceRate, CCRANDOM_0_1(), ccc4f(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 0.0f));

            distanceRate = pow(distanceRate, 3);

            tempLight->setWatchLayer(m_watchLayer, distanceRate);
            CCSize size = CCDirector::sharedDirector()->getVisibleSize();
            tempLight->setRelativePosition(ccp(size.width * CCRANDOM_0_1(), size.height + 100));
            this->addChild(tempLight);

            m_lastLightHeight = ralativeheight;
        }
        else
        {
            m_lastLightHeight = ralativeheight;
        }
    }

    CCLOG("%d", opacity);
}

void BackgroundLayer::reset()
{
    m_lastLightHeight = 0;
}