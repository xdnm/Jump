#ifndef __POWER_COLUMN__
#define __POWER_COLUMN__

#include "cocos2d.h"
#include "Utils/GlobalData.h"
USING_NS_CC;

class PowerColumn : public CCNode
{
public:
    ~PowerColumn();

    static PowerColumn* createPowerColumn();

    bool initPowerColumn();

    virtual void update(float delta);
    virtual void draw();

    void addPower(float power);
    int getPower();
    void reset();
protected:
    PowerColumn();

private:
    float m_time;
    float m_power;

    float m_maxPower;

    CCTexture2D *m_texture;
};
#endif