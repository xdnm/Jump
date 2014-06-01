#ifndef  __WEAPON__
#define __WEAPON__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "textures/CCTexture2D.h"
#include "Utils/TagHelper.h"
#include "Utils/B2WorldHelper.h"
//#include "Object/RoleObject.h"
//#include "Object/Monster.h"
using namespace std;
USING_NS_CC;

typedef struct _AttackAction
{
    _AttackAction()
    {
        rotation = 0.0f;
        rotationDur = 0.0f;
        moveToPos = ccp(0, 0);
        moveDur    = 0.0f;

    }
    float rotation;
    float rotationDur;

    CCPoint moveToPos;
    float moveDur;

    _AttackAction *preAction;
    _AttackAction *postAction;
} AttackAction;

//ActionGroup contains left and right version position and rotation data for an action.
class ActionGroup 
{
public:
    ActionGroup()
    {
        actionList = NULL;
    }
    ~ActionGroup()
    {
        m_leftVersion->release();
        m_rightVersion->release();

        AttackAction *temp = actionList;
        while (temp != NULL)
        {
            AttackAction *node = temp->postAction;
            delete temp;

            temp = node;
        }

    }
    ActionGroup(string actionName, CCFiniteTimeAction *leftVersion, CCFiniteTimeAction *rightVersion)
    {
        m_actionName = actionName;
        m_leftVersion = leftVersion;
        m_rightVersion = rightVersion;
    }
    CCFiniteTimeAction *GetLeftAction()
    {
        return m_leftVersion;
    }
    CCFiniteTimeAction *GetRightAction()
    {
        return m_rightVersion;
    }

    string ActionName;
    b2PolygonShape attackShape;
    float xOffset;

public:
    string m_actionName;
    CCFiniteTimeAction *m_leftVersion;
    CCFiniteTimeAction *m_rightVersion;

    AttackAction *actionList;
};


class Weapon : public cocos2d::CCSprite
{
public:
    Weapon();

    ~Weapon();

    void setUnit(float parm);
 

    virtual bool hook(b2Contact *contact, b2Body *otherBody) = 0;
    virtual bool attackAction() = 0;

    void setIsFaceLeft(bool flag);

    bool tryBeginAttack();
 
    void endAttack();
   

    void createActions();


   

    virtual bool initActions(){return false;};

    virtual void beginDamege(){};
    virtual void endDamege(){};

    bool tryPushAttackPool(CCNode *node);

    void clearAttackPool();

    inline CCPoint getWeaponPosition()
    {
        CCPoint point = m_hand->getPosition();
        if(m_isFaceLeft)
            point = point + ccp(-m_wave1->xOffset, 0);
        else
            point = point + ccp(m_wave1->xOffset, 0);

        return point;
    }
protected:
    inline CCPoint  DefaultPositon();
    inline float        DefaultRotation();
    inline CCPoint  WaveUpPosition();
    inline float        WaveUpRotation();
    inline CCPoint   WaveDownPosition();
    inline float        WaveDownRotation();




public:
    int m_damage;
    float m_criticalRate;

    //pointer to a biliboard which was passed from the role;
    BiliBoard *m_biliboard;

    CCNode* userdata;
protected:
    CCNode *m_hand;
    

    float m_unit;
    bool m_isFaceLeft;

    bool m_onAttacking;

    bool m_isHooked;




    CCSize m_size;
    CCPoint m_position;

    //default position and rotation
    CCPoint m_defaultPosiion;
    float m_defaultRotation;
    CCPoint m_waveUpPosition;
    float m_waveUpRotation;
    CCPoint m_waveDownPosition;
    float m_waveDownRotation;

    float m_attackSpeed;

    //attacking area
    b2Body *m_attackAreaBody;

    vector<int> *m_attackPool;


    ActionGroup *m_wave1;

};




#endif