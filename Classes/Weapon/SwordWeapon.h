#ifndef __SWORD_WEAPON__
#define __SWORD_WEAPON__

#include "Weapon/Weapon.h"
#include "Object/Monster.h"
#include "Visual/GUILayer.h"
class SwordWeapon : public Weapon
{
public:
    ~SwordWeapon();
    
    bool initModel();
    static SwordWeapon* createSword(CCNode *hand, float unit, b2Body *hookRoleBody, void *parm);

    bool initWithParm(CCNode *hand, float unit,b2Body *hookRoleBody,  void *parm = NULL);

    virtual bool attackAction();

    virtual bool hook(b2Contact *contact, b2Body *otherBody);
    virtual bool endHook();

    void createActions();
    virtual bool initActions();

    //begin to create the attacking area
    void beginDamege();
    //destroy the attacking area
    void endDamege();

    void updateAttackArea(float delta);

    void update(float delta);
    void updateWeapon(float dt);

    BiliBoard* interationWithOther(b2Contact *contact, b2Body* otherBody, bool onTouching);


private:
    SwordWeapon();
    b2Body *m_hookRoleBody;
    b2Body *m_hookedBody;
    b2JointDef *m_jointDef;

    CCMotionStreak *m_streak;
public:
private:
    CCTexture2D *m_texture;


};

#endif