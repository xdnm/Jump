#include "BrokenBlock.h"

BrokenBlock::BrokenBlock()
{

}

BrokenBlock::~BrokenBlock()
{

}

BrokenBlock* BrokenBlock::createBrokenBlock(xml_node<> *node)
{
    BrokenBlock* pRet = new BrokenBlock();

    if(pRet && pRet->initBrokenBlock(node))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool BrokenBlock::initBrokenBlock(xml_node<> *node)
{
    RubberBlock::initWithConfigNode(node);

    m_isEffective = true;

    return true;
}

void BrokenBlock::colliedWithRole(b2Contact *contact, b2Body *bodyOther)
{
    if(m_isEffective)
    {
        //this->setAlive(false);

        CCCallFunc *callfunc = CCCallFunc::create(this, callfunc_selector(BrokenBlock::setBlockBroken));
        float time = 0.4f;

        CCParticleSystemQuad *particle = CCParticleSystemQuad::create("expode.plist");
        //particle->setPosition(this->getB2NodePostion());
        particle->setDuration(0.2f);
        particle->setAutoRemoveOnFinish(true);
        this->addChild(particle);

        this->runAction(CCSequence::create(CCDelayTime::create(time), callfunc, NULL));

        
    }

    m_isEffective = false;
}

void BrokenBlock::setBlockBroken()
{
    this->setAlive(false);
}