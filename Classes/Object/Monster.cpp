#include "Monster.h"

Monster::Monster()
{
    B2CCNode::init();
}

Monster::~Monster()
{

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
        beenTrampled();
    }
    

    return;
}
