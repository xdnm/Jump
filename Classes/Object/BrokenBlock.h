#ifndef __BROKEN_BLOCK__
#define __BROKEN_BLOCK__

#include "RubberBlock.h"

class BrokenBlock : public RubberBlock
{
public:
    ~BrokenBlock();

    static BrokenBlock* createBrokenBlock(xml_node<> *node);

    bool initBrokenBlock(xml_node<> *node);

    void updateBlock(float dt);

    virtual void colliedWithRole(b2Contact *contact, b2Body *bodyOther);

    void setBlockBroken();
protected:
    BrokenBlock();
private:

    bool m_isEffective;
};
#endif