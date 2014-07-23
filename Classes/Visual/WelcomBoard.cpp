#include "WelcomBoard.h"

WelcomeBoard::WelcomeBoard()
{

}

WelcomeBoard::~WelcomeBoard()
{

}

bool WelcomeBoard::initWelcomeBoard()
{
    //this->initWithFile("go.png");
    this->init();
    CCSize size = CCDirector::sharedDirector()->getVisibleSize();

    CCLabelAtlas *label = CCLabelAtlas::create("0", "fonts/Text2.plist");
    label->setAnchorPoint(ccp(0.5f, 0.5f));

    label->setString("Tap to Go!");
    label->setScale((size.width * 0.75) / label->getContentSize().width);
    label->setPosition(0, 0);

    CCActionInterval *action = CCScaleBy::create(0.2f, 1.5f, 1.5f);
    
    label->runAction(CCRepeatForever::create(CCSequence::create(action, action->reverse(), CCDelayTime::create(2.0f), NULL)));
    
    this->addChild(label);
    //this->setScale((size.width / 3) / this->getContentSize().width);
    this->setAnchorPoint(ccp(0.5f, 0.5f));


    return true;
}

WelcomeBoard* WelcomeBoard::createWelcomeBoard()
{
    WelcomeBoard *pRet = new WelcomeBoard();

    if(pRet != NULL && pRet->initWelcomeBoard())
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