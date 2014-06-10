#ifndef __OBJECT_MODEL__
#define __OBJECT_MODEL__
#include "cocos2d.h"

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
    int m_strength;
    int m_agility;
    int m_strong;
    int m_firm;

    int m_health;
    int m_damege;
    int m_defence;
    float m_parry;
    float m_hitRatio;
};

#endif