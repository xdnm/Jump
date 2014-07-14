#include "Monster.h"

Monster::Monster()
{
    B2CCNode::init();
    m_model = new ObjectModel();

    m_isReady = true;
}

Monster::~Monster()
{
    delete m_model;
}

//attacking collied will be handled in the Weapon class, not in B2WorldHelper.
//So onCollied here only handle the role collied with the Monster not include the Attacking Collied.
void Monster::onCollied(b2Contact* contact, b2Body *bodyOther)
{
    CCNode *node;
    node = (CCNode *)(bodyOther->GetUserData());

    //if the node is null we do not handle it , for now.
    if(node == NULL)
        return ;

    if(TagHelper::Instance()->isObject(node->getTag(), ON_ROLE))
    {
        //beenTrampled();
    }
    

    return;
}

bool Monster::checkHealth()
{
    if(m_model->m_health <= 0)
    {
        CCLOG("monter tag: %d is dead!", this->getTag());
        m_layer->removeChild(this);

        return true;
    }
    return false;
}

void Monster::setReady()
{
    this->m_isReady = true;
}

