#ifndef __MOVE_BLOCK__
#define __MOVE_BLOCK__

#include "RubberBlock.h"
#include <math.h>

class MoveBlock : public RubberBlock
{
public:
    ~MoveBlock();
    static MoveBlock* createMoveBlock(xml_node<> *node, float timeCircle, float speed);

    bool initMoveBLock(xml_node<> *node,  float timeCircle, float speed);

    void updateMove(float dt);

    virtual void onCollied(b2Contact *contact, b2Body *bodyOther);

    virtual void setAlive(bool flag);
    virtual void colliedWithRole(b2Contact *contact, b2Body *bodyOther);

private:
    MoveBlock();

    float m_time;
    float m_timeCircle;
    float m_speed;

    bool m_stopMove;
};
#endif