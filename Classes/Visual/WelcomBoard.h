#ifndef __WELCOME_BOARD__
#define __WELCOME_BOARD__

#include "cocos2d.h"
USING_NS_CC;

class WelcomeBoard :public cocos2d::CCSprite
{
public:
    ~WelcomeBoard();

    static WelcomeBoard* createWelcomeBoard();

    bool initWelcomeBoard();
protected:
    WelcomeBoard();
private:
};

#endif