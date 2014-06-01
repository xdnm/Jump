#include "Weapon.h"

Weapon::Weapon()
{
    m_size = CCSizeMake(0, 0);
    m_position = ccp(0, 0);
    m_unit = 1.0f;

    m_isFaceLeft = true;
    m_onAttacking = false;
    m_isHooked = false;

    //B2Helper::Instance()->getWorld()->SetContactListener((b2ContactListener*)(this));
    this->setTag(TagHelper::Instance()->getTag(ON_WEAPON));

    m_attackPool = new vector<int>();

    m_attackAreaBody = NULL;
}

Weapon::~Weapon()
{

}

void Weapon:: setUnit(float parm)
{
    m_unit = 1.0f;
}

void Weapon:: setIsFaceLeft(bool flag)
{
    if(m_isFaceLeft != flag)
    {
        m_isFaceLeft = flag;
    }
}
bool Weapon:: tryBeginAttack()
{
    if(!m_onAttacking)
    {

        m_onAttacking = true;
        return true;
    }
    else
        return false;
}
void Weapon:: endAttack()
{
    /*m_onAttacking = false;
    clearAttackPool();*/
}

bool Weapon::tryPushAttackPool(CCNode *node)
{
    vector<int>::iterator iter;
    for(iter = m_attackPool->begin(); iter != m_attackPool->end(); iter++)
    {
        if(*iter == (int)node)
            break;
    }

    if(iter != m_attackPool->end())
    {
        return false;
    }
    else
    {
        m_attackPool->push_back((int)node);
        return true;
    }
}
void Weapon::clearAttackPool()
{
    m_attackPool->clear();
}

//
//CCPoint Weapon::getWeaponPosition()
