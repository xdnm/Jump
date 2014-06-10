#include "ObjectModel.h"

ObjectModel::ObjectModel()
{
    m_strong = 0;
    m_strength = 0;
    m_agility = 0;
    m_firm = 0;

    m_health = 10;
    m_damege = 10;
    m_defence = 10;
    m_parry = 0.05f;
    m_hitRatio = 1;
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
    m_damege = m_damege * m_strength / 100;
    m_defence = m_defence * m_strong / 100;
    m_parry = m_parry * m_agility / 100;
    m_hitRatio = m_hitRatio * m_agility / 100;
    m_health = m_health * m_strong / 100;
}

void ObjectModel::plusAModel(ObjectModel *otherModel)
{
    m_health     += otherModel->m_health;
    m_damege  += otherModel->m_damege;
    m_defence  += otherModel->m_defence;
    m_parry       +=otherModel->m_parry;
    m_hitRatio   += otherModel->m_hitRatio;

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
}

bool ObjectModel::checkIsDead()
{
    return m_health <= 0;
}