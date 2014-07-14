#ifndef _BLOCK_
#define _BLOCK_

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "Utils/B2WorldHelper.h"
#include "Utils/TagHelper.h"
#include "Object/B2CCNode.h"
#include "Object/RoleObject.h"
USING_NS_CC;

class Block : public B2CCNode
{
public:
	Block();
	virtual void setAlive(bool flag){};
	virtual bool setBlockPosition(cocos2d::CCPoint position){return true;};
    virtual bool tryLaunchParticle(CCPoint emitPoint){return true;};

private:
	virtual bool createBody(){return true;};
	virtual bool initRenderData(){return true;};
	

protected:

};

#endif