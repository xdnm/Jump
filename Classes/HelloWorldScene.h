#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "Utils/GLES-Render.h"
#include "Utils/GlobalData.h"
#include "Utils/B2WorldHelper.h"
#include "Utils/JumpFollow.h"
#include "Utils/ForeSceneManager.h"
#include "Utils/B2Handler.h"
#include "Object/RoleObject.h"
#include "Weapon/SwordWeapon.h"


class HelloWorld : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

	virtual void draw();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);

	//virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    void didAccelerate(CCAcceleration* pAccelerationValue);

    void copyData(const char* pFileName);
    bool isFileExist(const char* pFileName)
    {
        if( !pFileName ) return false;

        //strFilePathName is :/data/data/ + package name
        std::string filePath = CCFileUtils::sharedFileUtils()->getWritablePath();
        filePath += pFileName;

        FILE *fp = fopen(filePath.c_str(),"r");
        if(fp)
        {
            fclose(fp);
            return true;
        }
        return false;
    }

private:
	void worldTick(float dt);

private:
	b2World *m_world;
	RoleObject *m_role;
};

#endif // __HELLOWORLD_SCENE_H__
