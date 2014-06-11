#ifndef __OBJECT_MODEL__
#define __OBJECT_MODEL__
#include "cocos2d.h"
#include "Object/B2CCNode.h"
USING_NS_CC;

class ObjectModel 
{
public:
    ObjectModel();
    ~ObjectModel();

    void selfInit();

    void plusAModel(ObjectModel *otherModel);

    void beenAttackWithModel(ObjectModel *otherModel);

    void build();

    inline int getStrong()
    {
        return m_strong;
    }

    inline void setStrong(int temp)
    {
        m_strong = temp;
        build();
    }

    inline int getAgility()
    {
        return m_agility;
    }
    inline void setAgility(int temp)
    {
        m_agility = temp;
        build();
    }

    inline int getStrength()
    {
        return m_strength;
    }
    inline void setStrength(int temp)
    {
        m_strength = temp;
        build();
    }

    inline int getFirm()
    {
        return m_firm;
    }
    inline void setFirm(int temp)
    {
        m_firm = temp;
        build();
    }

    bool checkIsDead();
protected:
public:
    int d_health;
    int d_damege;
    int d_defence;
    int d_parry;
    int d_hitRatio;

    int m_strength;
    int m_agility;
    int m_strong;
    int m_firm;

    int m_health;
    int m_damege;
    int m_defence;
    int m_parry;
    int m_hitRatio;

    ///
    ///pointer to the B2Node which holds this model.
    ///In this way, model can add visiual effect and physical effect to the b2node.
    B2CCNode *m_B2Node;
};

#endif