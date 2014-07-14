#include "WelcomBoard.h"

WelcomeBoard::WelcomeBoard()
{

}

WelcomeBoard::~WelcomeBoard()
{

}

bool WelcomeBoard::initWelcomeBoard()
{
    this->initWithFile("go.png");
    CCSize size = CCDirector::sharedDirector()->getVisibleSize();
    this->setScale((size.width / 3) / this->getContentSize().width);
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