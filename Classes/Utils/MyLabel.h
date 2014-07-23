#ifndef __MYLABEL__
#define __MYLABEL__

#include "cocos2d.h"
USING_NS_CC;

class MyLabel : public cocos2d::CCLabelAtlas
{
public:
    static CCLabelAtlas* createMyLabel(const char* str);

    bool initMyLabel(const char* str);
    ~MyLabel();
protected:
    MyLabel();
};
#endif