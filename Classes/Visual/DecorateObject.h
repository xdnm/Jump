#ifndef __DECORATE_OBJECT__
#define __DECORATE_OBJECT__

#include "cocos2d.h"

USING_NS_CC;

class DecorateObject : public cocos2d::CCSprite
{
public:
     ~DecorateObject();

     static DecorateObject* createDecObject(const char* filename, int distance, CCLayer *layer);

     bool initDecobject(const char*filename,int distance, CCLayer *layer);

     void lifeUpdate(float dt);
protected:
    DecorateObject();
private:

    CCLayer *m_layer;
    
    int m_layerInitHeight;
    int m_distance;
};
#endif