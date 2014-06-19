#ifndef __ROLE_OBJECT__
#define  __ROLE_OBJECT__


#include "cocos2d.h"
#include "textures/CCTexture2D.h"
#include "Box2D/Box2D.h"
#include "Object/B2CCNode.h"
#include "Utils/GlobalData.h"
#include "Utils/B2WorldHelper.h"
#include "Utils/TagHelper.h"
#include "Weapon/SwordWeapon.h"
#include "Object/Monster.h"
#include "Model/ObjectModel.h"
USING_NS_CC;

class RoleObject: public B2CCNode
{
public:
	static RoleObject* CreateRole(b2World* world, void *parm);
    ~RoleObject();
	
    /**After all the components was initialized, we can init the model in this function.
        And we also provide this B2Node with enough information,like mainBody, visualNode.
        At last we need to make a reference between model and model-hold-b2node.
    */
    bool initModel();

	virtual void jump(float massRatio = 1.0f);
	virtual void onCollied(b2Contact *contact, b2Body *bodyOther);

    bool attack();
    void onAttacking(b2Contact *contact, b2Body* otherBody);

    //lay touch related functions.
    bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    bool onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    bool onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);

    void setLinearVecByAcceleration(CCAcceleration* pAccelerationValue);

private:
	RoleObject();
    void update(float delta);

	bool initWithWorld(b2World* world, void *parm);

	void createBody(void *parm);

	void createDistanceJoint(b2Body *bodyA, b2Body *bodyB, float dampRatio = 0.5f, float freq = 4.0f);
	void createRevoluteJoint(b2Body *bodyA, b2Body *bodyB, float angle);

	virtual void draw()	;

    void hookAction(b2Body *hookBody, b2Vec2 point);

    void setFaceLeft(bool isFaceLeft);

    void beenAttacked(Monster *monster);
    bool isProtected();
    void setUnProtected();


public:
    //ccnode for layer to follow or other things
    CCNode *m_node;

    ObjectModel *m_model;

protected:
private:
    float m_length;
    bool m_bounce;

	GLfloat m_innerRadius;
	GLfloat m_outRadius;
	
	b2World *m_world;
	CCTexture2D *m_texture;
	
	//body parts
	b2Body *m_topBoby;
	b2Body *m_bottomBody;
	b2Body *m_leftBody;
	b2Body *m_rightBody;
	b2Body *m_innerBody;

	//render data
	Vertex2D m_vertexCoords[6];
	Vertex2D m_textureCoords[6];
	
    CCSprite *m_leftHandSprite;
    CCSprite *m_rightHandSprite;

    //weapon data
    Weapon *m_weapon;


    float m_Unit;
    bool m_faceLeft;

    //touch relatied.
    bool m_onTouchDown;

    bool m_isProtected;
};

#endif;