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
#include "Visual/BackgroundLayer.h"
#include "Visual/MyCCMotionStreak.h"
#include "Visual/ScoreBoard.h"
#include "Visual/GUILayer.h"


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

    void setIsStop(bool stop)
    {
        if(stop)
        {
            this->unschedule(schedule_selector(HelloWorld::worldTick));
        }
        else
        {
            this->schedule(schedule_selector(HelloWorld::worldTick), 0.1f);
        }
    }

public:
        BackgroundLayer *m_background;
        GUILayer *m_guiLayer;

        void createWelcome();
        void createNewGame();
        inline B2CCNode* GetRole()
        {
            return m_role;
        }

        void reset();

        void setGameState(GameState state);

private:
	void worldTick(float dt);

private:
	b2World *m_world;
	RoleObject *m_role;


    MyCCMotionStreak *m_streak;

    GameState m_state;
};

#endif // __HELLOWORLD_SCENE_H__
