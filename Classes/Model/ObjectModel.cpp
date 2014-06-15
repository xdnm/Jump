#include "ObjectModel.h"

ObjectModel::ObjectModel()
{
    m_strong = 0;
    m_strength = 0;
    m_agility = 0;
    m_firm = 0;

    d_health = 0;
    d_damege = 0;
    d_defence = 0;
    d_parry = 0;
    d_hitRatio = 0;

    m_health = 0;
    m_damege = 0;
    m_defence = 0;
    m_parry = 0;
    m_hitRatio = 0;
};
ObjectModel::~ObjectModel()
{

}

void ObjectModel::selfInit()
{
    m_strong = 5;
    m_strength = 5;
    m_agility = 5;
    m_firm = 5;

}

void ObjectModel::build()
{
    /*m_damege = d_damege + d_damege * ((float)m_strength / 100);
    m_defence = d_defence + d_defence * ((float)m_strong / 100);
    m_parry = d_parry + d_parry * ((float)m_agility / 100);
    m_hitRatio = d_hitRatio + d_hitRatio * ((float)m_agility / 100);
    m_health = d_health +  d_health * ((float)m_strong / 100);*/

    m_damege = d_damege + m_strength / 2 + m_strong / 6;
    m_defence = d_defence + m_strong / 2;
    m_parry = d_parry + m_agility / 2;
    m_hitRatio = d_hitRatio + m_agility / 2;
    m_health = d_health + m_firm / 2 + m_strong / 6;
}

void ObjectModel::plusAModel(ObjectModel *otherModel)
{
    d_health     += otherModel->d_health;
    d_damege  += otherModel->d_damege;
    d_defence  += otherModel->d_defence;
    d_parry       +=otherModel->d_parry;
    d_hitRatio   += otherModel->d_hitRatio;

    m_strength  += otherModel->m_strength;
    m_strong     += otherModel->m_strong;
    m_agility      += otherModel->m_agility;
    m_firm         += otherModel->m_firm;

    build();
}

void ObjectModel::beenAttackWithModel(ObjectModel *otherModel)
{
    int damege;
    damege = otherModel->m_damege - m_defence;

    damege = damege > 0 ? damege : 1;

    m_health -= damege;
    
    m_B2Node->createDamegeNum(ccp(0, 0), damege);
    CCLOG("damege: %d, health : %d", damege, m_health);
}

bool ObjectModel::checkIsDead()
{
    return m_health <= 0;
}