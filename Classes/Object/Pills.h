#ifndef __PILLS__
#define __PILLS__


#include "cocos2d.h"
#include "Object/B2CCNode.h"
#include "Utils/GlobalData.h"
#include "Utils/B2WorldHelper.h"
#include "Utils/TagHelper.h"

USING_NS_CC;

enum PillType
{
    PT_RED = 1,
    PT_GREEN,
    PT_BLUE,
    PT_YELLOW,
    PT_NONE
};

class Pill :public B2CCNode
{
public:
    Pill();
    ~Pill();

    static Pill* createPillWithType(PillType type, CCPoint position);

    bool initPillWithType(PillType type, CCPoint position);

    inline PillType getType()
    {
        return m_pillType;
    }

    void releaseUpdate(float dt);

    virtual void onCollied(b2Contact* contact, b2Body *bodyOther);

    bool IsAffective();
    void setIsAffective(bool flag);

protected:
private:
    PillType m_pillType;

    b2Body *m_polyBody;

    bool m_isAffective;
};
#endif