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
    m_fadeInInitHeight = 0;
    m_fadeInBackground = NULL;
    m_fadeOutBackground = NULL;

    decorate = NULL;
    //m_background1 = CCSprite::create("background3.png");
    //m_background2 = CCSprite::create("background4.png");

    //m_background1->setAnchorPoint(ccp(0, 0));
    //m_background2->setAnchorPoint(ccp(0, 0));

    //m_background1->setScale(CCDirector::sharedDirector()->getVisibleSize().width / m_background1->getContentSize().width);
    //m_background2->setScaleX(CCDirector::sharedDirector()->getVisibleSize().width / m_background2->getContentSize().width);
    //m_background2->setScaleY(CCDirector::sharedDirector()->getVisibleSize().height / m_background2->getContentSize().height);

    scene->addChild(this, -10);

    //CircleLight *light = CircleLight::createCircleLight(80, 100);
    CircleLight *light = CircleLight::createCircleLight(30, 60, 5.0f, 0.1f, ccc4f(1.0f, 0.0f, 0.0f, 0.0f));
    light->setWatchLayer(m_watchLayer, 100);
    light->setRelativePosition(ccp(100, 500));

    CircleLight *light1 = CircleLight::createCircleLight(80, 90, 3.0f, 0.5f, ccc4f(0.8f, 0.0f, 0.0f, 0.0f));
    light1->setWatchLayer(m_watchLayer, 10);
    light1->setRelativePosition(ccp(400, 400));


    reset();
    this->addChild(light1);
    this->addChild(light);
    this->scheduleUpdate();
    return true;
}

void BackgroundLayer::update(float dt)
{
    height = m_watchLayer->getPositionY();
    int ralativeheight = -height - m_fadeInInitHeight;

    int opacity = ralativeheight/400;
    if(opacity > 255)
    {
        m_fadeInInitHeight = ralativeheight;

        CCSprite *temp;
        temp = m_fadeInBackground;
        m_fadeInBackground = m_fadeOutBackground;
        m_fadeOutBackground = temp;

        m_fadeInBackground->initWithFile("backgroundNight.png");
        m_fadeInBackground->setAnchorPoint(ccp(0.0f, 0.0f));
        m_fadeInBackground->setScaleX(CCDirector::sharedDirector()->getVisibleSize().width / m_fadeInBackground->getContentSize().width);
        m_fadeInBackground->setScaleY(CCDirector::sharedDirector()->getVisibleSize().height / m_fadeInBackground->getContentSize().height);
        
    }
    else 
    {
        m_fadeOutBackground->setOpacity(255 - opacity);
        m_fadeInBackground->setOpacity(opacity);
    }
   

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

    if(-height > 80000 && decorate == NULL)
    {
        decorate = DecorateObject::createDecObject("moon.png", 300, m_watchLayer);
        decorate->setPosition(ccp(CCDirector::sharedDirector()->getVisibleSize().width / 3, CCDirector::sharedDirector()->getVisibleSize().height));
        decorate->setScale(CCDirector::sharedDirector()->getVisibleSize().width / decorate->getContentSize().width);
        decorate->setAnchorPoint(ccp(0, 0));
        this->addChild(decorate, -10);
    }
    //CCLOG("%d", opacity);
}

void BackgroundLayer::reset()
{
    m_lastLightHeight = 0;

    m_fadeInInitHeight = 0;

    if(decorate != NULL)
    {
        decorate->removeFromParent();
        decorate = NULL;
    }
    resetBackground();
}

void BackgroundLayer::resetBackground()
{
    if(m_fadeOutBackground != NULL)
        m_fadeOutBackground->removeFromParent();

    if(m_fadeInBackground != NULL)
        m_fadeInBackground->removeFromParent();

    m_fadeInBackground = CCSprite::create("background4.png");
    m_fadeOutBackground = CCSprite::create("background3.png");

    m_fadeInBackground->setAnchorPoint(ccp(0, 0));
    m_fadeOutBackground->setAnchorPoint(ccp(0, 0));

    CCSize size = CCDirector::sharedDirector()->getVisibleSize();
    m_fadeInBackground->setScaleX(size.width / m_fadeInBackground->getContentSize().width);
    m_fadeInBackground->setScaleY(size.height / m_fadeOutBackground->getContentSize().height);
    m_fadeOutBackground->setScaleX(size.width / m_fadeOutBackground->getContentSize().width);
    m_fadeOutBackground->setScaleY(size.height / m_fadeOutBackground->getContentSize().height);

    this->addChild(m_fadeInBackground, -5000);
    this->addChild(m_fadeOutBackground, -5001);
}