#ifndef _RIGID_BODY_
#define _RIGID_BODY_
#include "Block.h"
#include "textures/CCTexture2D.h"
#include "Utils/GlobalData.h"
#include "Utils/B2WorldHelper.h"
#include "Utils/TagHelper.h"


class RigidBlock : public Block
{
public:
	RigidBlock();
	~RigidBlock();

	static RigidBlock* createRigidBlock(CCPoint point, CCSize size, void *parm);
	void setAlive(bool flag);
	bool setBlockPosition(cocos2d::CCPoint position);
	void interationWithOther(b2Body* otherBody);

    /**this function will be called when we user setB2NodePostion to change the position*/
    void onB2PositionChanged();
	
	void draw();

	void onCollied(b2Contact *contact, b2Body *bodyOther);
private:
	bool initWithPointSize(CCPoint point, CCSize size, void *parm);
	bool createBody();
	bool initRenderData();

    void makeRoleJump(float dt);
protected:


private:
	b2Body *m_polyBody;
	CCTexture2D *m_texture;
	
	//render data
	Vertex2D m_textCoord[4];
	Vertex2D m_vertexCoord[4];
};

#endif