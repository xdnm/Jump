#include "Utils/JumpFollow.h"
//
// Follow
//
JumpFollow::~JumpFollow()
{
    CC_SAFE_RELEASE(m_pobFollowedNode);
}

JumpFollow* JumpFollow::create(CCNode *pFollowedNode, const CCRect& rect/* = CCRectZero*/)
{
    JumpFollow *pRet = new JumpFollow();
    if (pRet && pRet->initWithTarget(pFollowedNode, rect))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool JumpFollow::initWithTarget(CCNode *pFollowedNode, const CCRect& rect/* = CCRectZero*/)
{
    CCAssert(pFollowedNode != NULL, "");

    pFollowedNode->retain();
    m_pobFollowedNode = pFollowedNode;
    if (rect.equals(CCRectZero))
    {
        m_bBoundarySet = false;
    }
    else
    {
        m_bBoundarySet = true;
    }

    m_bBoundaryFullyCovered = false;

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    m_obFullScreenSize = CCPointMake(winSize.width, winSize.height);
    m_obHalfScreenSize = ccpMult(m_obFullScreenSize, 0.5f);


    if (m_bBoundarySet)
    {
        m_fLeftBoundary = -((rect.origin.x+rect.size.width) - m_obFullScreenSize.x);
        m_fRightBoundary = -rect.origin.x ;
        m_fTopBoundary = -rect.origin.y;
        m_fBottomBoundary = -((rect.origin.y+rect.size.height) - m_obFullScreenSize.y);

        if(m_fRightBoundary < m_fLeftBoundary)
        {
            // screen width is larger than world's boundary width
            //set both in the middle of the world
            m_fRightBoundary = m_fLeftBoundary = (m_fLeftBoundary + m_fRightBoundary) / 2;
        }
        if(m_fTopBoundary < m_fBottomBoundary)
        {
            // screen width is larger than world's boundary width
            //set both in the middle of the world
            m_fTopBoundary = m_fBottomBoundary = (m_fTopBoundary + m_fBottomBoundary) / 2;
        }

        if( (m_fTopBoundary == m_fBottomBoundary) && (m_fLeftBoundary == m_fRightBoundary) )
        {
            m_bBoundaryFullyCovered = true;
        }
    }

    return true;
}

CCObject *JumpFollow::copyWithZone(CCZone *pZone)
{
    CCZone *pNewZone = NULL;
    JumpFollow *pRet = NULL;
    if(pZone && pZone->m_pCopyObject) //in case of being called at sub class
    {
        pRet = (JumpFollow*)(pZone->m_pCopyObject);
    }
    else
    {
        pRet = new JumpFollow();
        pZone = pNewZone = new CCZone(pRet);
    }
    CCAction::copyWithZone(pZone);
    // copy member data
    pRet->m_nTag = m_nTag;
    CC_SAFE_DELETE(pNewZone);
    return pRet;
}

void JumpFollow::step(float dt)
{
    CC_UNUSED_PARAM(dt);

    if(m_bBoundarySet)
    {
        // whole map fits inside a single screen, no need to modify the position - unless map boundaries are increased
        if(m_bBoundaryFullyCovered)
            return;

        CCPoint tempPos = ccpSub( m_obHalfScreenSize, m_pobFollowedNode->getPosition());

        m_pTarget->setPosition(ccp(clampf(tempPos.x, m_fLeftBoundary, m_fRightBoundary), 
            clampf(tempPos.y, m_fBottomBoundary, m_fTopBoundary)));
    }
    else
    {
        //m_pTarget->setPosition(ccpSub(m_obHalfScreenSize, m_pobFollowedNode->getPosition()));
        //CCPoint tempPos = ccpSub( m_pTarget->getPosition() + ccp(CCDirector->getVisibleSize().width, ), m_pobFollowedNode->getPosition());
        
       /* CCPoint tempPos =m_pobFollowedNode->getPosition();
        int midHeight = m_pTarget->getPositionY() + m_obHalfScreenSize.y;
        m_pTarget->setPosition(0, tempPos.y > midHeight ? tempPos.y : midHeight);*/
        CCPoint halfScreenPoint = -m_pTarget->getPosition() + m_obHalfScreenSize;
        int height = halfScreenPoint.y - m_pobFollowedNode->getPositionY();

        if(height < 0)
            m_pTarget->setPositionY(m_pTarget->getPositionY() + height);
    }
}

bool JumpFollow::isDone()
{
    return ( !m_pobFollowedNode->isRunning() );
}

void JumpFollow::stop()
{
    m_pTarget = NULL;
    CCAction::stop();
}
