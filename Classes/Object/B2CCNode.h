#ifndef __B2CCNODE__
#define __B2CCNODE__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "Utils/BiliBoard.h"
USING_NS_CC;

class B2CCNode : public CCNode
{
public:
    B2CCNode()
    {
        m_position = ccp(0, 0);
        m_size = CCSizeMake(0, 0);
        m_isAlive = false;

        m_biliBoard = NULL;
    }
    virtual void setAlive(bool flag){};

    bool inline isAlive()
    {
        return m_isAlive;
    }

    CCPoint getB2NodePostion()
    {
        return m_position;
    }

    void setB2NodePostion(CCPoint point)
    {
        m_position = point;
        setAlive(true);
    }

    CCSize getB2NodeSize()
    {
        return m_size;
    }

    virtual void onCollied(b2Contact* contact, b2Body *bodyOther) {};
    virtual void buildBiliBoard(){};
    virtual BiliBoard* getBiliBoard(){return NULL;};
    virtual void reciveBiliBoard(BiliBoard *board){};
protected:
    CCPoint m_position;
    CCSize m_size;
    bool m_isAlive;

    BiliBoard *m_biliBoard;
};


#endif