#ifndef _JUMP_FOLLOW_
#define _JUMP_FOLLOW_

#include "cocos2d.h"
#include "actions/CCAction.h"
USING_NS_CC;

class  JumpFollow : public CCAction
{
public:
    /**
     *  @js ctor
     */
    JumpFollow()
        : m_pobFollowedNode(NULL)
        , m_bBoundarySet(false)
        , m_bBoundaryFullyCovered(false)        
        , m_fLeftBoundary(0.0)
        , m_fRightBoundary(0.0)
        , m_fTopBoundary(0.0)
        , m_fBottomBoundary(0.0)
    {}
    /**
     *  @js NA
     *  @lua NA
     */
    virtual ~JumpFollow(void);
    
    inline bool isBoundarySet(void) { return m_bBoundarySet; }
    /** alter behavior - turn on/off boundary */
    inline void setBoudarySet(bool bValue) { m_bBoundarySet = bValue; }

    /** initializes the action with a set boundary */
    bool initWithTarget(CCNode *pFollowedNode, const CCRect& rect = CCRectZero);
    /**
     *  @js NA
     *  @lua NA
     */
    virtual CCObject* copyWithZone(CCZone *pZone);
    virtual void step(float dt);
    virtual bool isDone(void);
    virtual void stop(void);

public:
    /** creates the action with a set boundary,
    It will work with no boundary if @param rect is equal to CCRectZero.
    */
    static JumpFollow* create(CCNode *pFollowedNode, const CCRect& rect = CCRectZero);
protected:
    // node to follow
    CCNode *m_pobFollowedNode;

    // whether camera should be limited to certain area
    bool m_bBoundarySet;

    // if screen size is bigger than the boundary - update not needed
    bool m_bBoundaryFullyCovered;

    // fast access to the screen dimensions
    CCPoint m_obHalfScreenSize;
    CCPoint m_obFullScreenSize;

    // world boundaries
    float m_fLeftBoundary;
    float m_fRightBoundary;
    float m_fTopBoundary;
    float m_fBottomBoundary;
};

#endif