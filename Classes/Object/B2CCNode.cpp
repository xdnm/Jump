#include "B2CCNode.h"

void B2CCNode::createDamegeNum(CCPoint point, int num, bool isCritical /* = false */, float dur /* = 1.0f */)
{
    if(m_visiableNode == NULL)
        return;
    char s[10];
    sprintf(s, "%d", num);
    CCLabelAtlas *label2 = CCLabelAtlas::create(s, "fonts/tuffy_bold_italic-charmap.plist");  
    m_visiableNode->addChild(label2, 0);
    label2->setPosition(point);  
    label2->setOpacity(200);  
    label2->setColor( ccRED ); 
    label2->setScale(50/label2->getContentSize().height);
    label2->setAnchorPoint(ccp(0.5f, 0.0f));
    CCActionInterval *action = CCMoveTo::create(2.0f, ccp(10, 400));
    CCCallFuncN *callFunc = CCCallFuncN::create(this, callfuncN_selector(B2CCNode::deleteDamegeNum));
    CCFiniteTimeAction *seq = CCSequence::create(action, callFunc, NULL);
    label2->runAction(seq);
}

void B2CCNode::deleteDamegeNum(CCNode *numNode)
{
    if(numNode != NULL)
    {
        m_visiableNode->removeChild(numNode, false);
        //do not know why do i have do a autorelease in here, otherwise, there will be a block error when proj closing.
        numNode->autorelease();
    }
}