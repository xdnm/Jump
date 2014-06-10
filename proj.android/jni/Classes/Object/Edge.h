#ifndef  __EDGE__
#define __EDGE__

#include "Object/B2CCNode.h"
#include "Utils/TagHelper.h"

USING_NS_CC;

class Edge : public B2CCNode
{
public:
    Edge()
    {
        setTag(TagHelper::Instance()->getTag(ON_EDGE));
    }


public:
    static int s_leftX;
    static int s_rightX;
    static int s_height;
protected:
    bool m_isLeft;
};



#endif