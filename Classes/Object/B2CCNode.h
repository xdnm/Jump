#ifndef __B2CCNODE__
#define __B2CCNODE__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
USING_NS_CC;

class B2CCNode : public CCNode
{
public:
    B2CCNode()
    {
        m_position = ccp(0, 0);
        m_size = CCSizeMake(0, 0);
        m_isAlive = false;
    }

    ~B2CCNode()
    {
        //m_visiableNode->release();
    }

    /**after all the components was initialized, we can init the model in this function*/
    virtual bool initModel(){return true;};
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
public:
        //main body, we can manipulate it to move this B2CCNode
        b2Body *m_mainBody;
        CCSize m_size;
        CCSprite *m_visiableNode;

protected:
    CCPoint m_position;
    bool m_isAlive;


};


#endif