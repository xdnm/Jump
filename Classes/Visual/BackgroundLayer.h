#include "cocos2d.h"
#include "CircleLight.h"
USING_NS_CC;

class BackgroundLayer :public CCLayer
{
public:
    ~BackgroundLayer();
    static BackgroundLayer* createBackground(CCScene *scene, CCLayer *watchLayer);
    bool initBackground(CCScene *scene, CCLayer *wachLayer);

    void update(float dt);
    
protected:
    BackgroundLayer();
private:
    CCLayer *m_watchLayer;

    CCSprite *m_background1;
    CCSprite *m_background2;

    int height;
};
