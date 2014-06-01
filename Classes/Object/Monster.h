#ifndef __MONSTOR__
#define __MONSTOR__

#include "cocos2d.h"
#include "textures/CCTexture2D.h"
#include "Box2D/Box2D.h"
#include "Utils/B2WorldHelper.h"
#include "Utils/TagHelper.h"
#include "Weapon/Weapon.h"

USING_NS_CC;

class Monster : public B2CCNode
{
public:
    ~Monster();
    Monster();
    virtual bool beenTrampled(){ return true; };

    virtual bool beenAttacked(Weapon *weapon){return true;};
    virtual bool beenHooked(Weapon *weapon){return true;};

    virtual void onCollied(b2Contact* contact, b2Body *bodyOther);
protected:
    int m_damege;
    int m_defence;

};

#endif