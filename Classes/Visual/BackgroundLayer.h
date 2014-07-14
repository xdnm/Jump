#ifndef __BACKGROUND_LAYER__
#define __BACKGROUND_LAYER__
#include "cocos2d.h"
#include "CircleLight.h"
USING_NS_CC;

class BackgroundLayer :public CCLayer
{
public:
    ~BackgroundLayer();
    static BackgroundLayer* createBackground(CCNode *scene, CCLayer *watchLayer);
    bool initBackground(CCNode *scene, CCLayer *wachLayer);

    void update(float dt);

    void reset();
    
protected:
    BackgroundLayer();
private:
    CCLayer *m_watchLayer;

    CCSprite *m_background1;
    CCSprite *m_background2;

    int m_maxLight;
    int m_lightNum;
    int m_lastLightHeight;
    int height;
};
#endif
