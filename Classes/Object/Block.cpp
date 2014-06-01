#include "Block.h"

Block::Block()
{
	CCNode::init();

    this->setTag(TagHelper::Instance()->getTag(ON_BLOCK));
}