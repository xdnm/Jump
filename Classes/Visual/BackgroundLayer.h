#ifndef __BACKGROUND_LAYER__
#define __BACKGROUND_LAYER__
#include "cocos2d.h"
#include "CircleLight.h"
#include "Visual/DecorateObject.h"
USING_NS_CC;

class BackgroundLayer :public CCLayer
{
public:
    ~BackgroundLayer();
    static BackgroundLayer* createBackground(CCNode *scene, CCLayer *watchLayer);
    bool initBackground(CCNode *scene, CCLayer *wachLayer);

    void update(float dt);

    void reset();
    
    void resetBackground();
protected:
    BackgroundLayer();
private:
    CCLayer *m_watchLayer;

    //need deleted
    CCSprite *m_background1;
    CCSprite *m_background2;

    CCSprite *m_fadeInBackground;
    CCSprite *m_fadeOutBackground;

    DecorateObject *decorate;

    int m_fadeInInitHeight;
    int m_fadeOutInitHeight;

    int m_maxLight;
    int m_lightNum;
    int m_lastLightHeight;
    int height;
};
#endif
