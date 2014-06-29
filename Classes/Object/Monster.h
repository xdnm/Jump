#ifndef __MONSTOR__
#define __MONSTOR__

#include "cocos2d.h"
#include "textures/CCTexture2D.h"
#include "Box2D/Box2D.h"
#include "Utils/B2WorldHelper.h"
#include "Utils/TagHelper.h"
#include "Weapon/Weapon.h"
#include "Model/ObjectModel.h"

USING_NS_CC;

class Monster : public B2CCNode
{
public:
    ~Monster();
    Monster();

    virtual bool beenTrampled(ObjectModel *model){ return true; };
    virtual bool attacked(ObjectModel *model){return true;};
    //bool subAttacked(ObjectModel *model);
    virtual bool beenHooked(ObjectModel *model){return true;};
    virtual bool beenAttacked(ObjectModel *model){return true;};

    virtual void onCollied(b2Contact* contact, b2Body *bodyOther);
    
    bool checkHealth();

    /**Whether this monster is ready is attack role and ready to beenTrample may be affect by many conditions
        overload this function is subclass */
    virtual bool isReady(){return m_isReady;};
    void setReady();
public:
    ObjectModel *m_model;
    CCLayer *m_layer;

protected:
    bool m_isReady;
};

#endif