#ifndef __B2CCNODE__
#define __B2CCNODE__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "Utils/RapidXml.h"
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
        //this == NULL;
        //m_visiableNode->release();
    }

    /**after all the components was initialized, we can init the model in this function*/
    virtual bool initModel(){return true;};
    virtual void setAlive(bool flag){};
    virtual void onB2PositionChanged(){};

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
        //setAlive(true);
        onB2PositionChanged();
    }

    CCSize getB2NodeSize()
    {
        return m_size;
    }

    /**create a demage num on this B2CCNode , point is the self coordinate point*/
    void createDamegeNum(CCPoint point, int num, bool isCritical = false, float dur = 1.0f);
    void deleteDamegeNum(CCNode *numNode);
    virtual void onCollied(b2Contact* contact, b2Body *bodyOther) {};
public:
        //main body, we can manipulate it to move this B2CCNode
        b2Body *m_mainBody;
        CCSize m_size;
        CCNode *m_visiableNode;

protected:
    CCPoint m_position;
    bool m_isAlive;


};


#endif