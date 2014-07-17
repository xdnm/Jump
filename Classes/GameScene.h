#ifndef __GAME_SCENE__
#define __GAME_SCENE__

#include "cocos2d.h"
#include "Visual/BackgroundLayer.h"
#include "Visual/GUILayer.h"
#include "HelloWorldScene.h"
#include "Utils/GlobalData.h"
#include "Utils/SoundManager.h"
#include "Visual/WelcomBoard.h"
#include "Visual/DeadEndBoard.h"
USING_NS_CC;



class GameScene :public CCLayer
{
public:
    ~GameScene();

    static GameScene* Instance();

    static CCScene *scene();
protected:
    GameScene();

    void CreateNewScene(void *parm);

    void gameUpdate(float dt);

    void setGameState(GameState state);

    void welcome();

    void roleDead();

    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);

    virtual void keyBackClicked();//Android ·µ»Ø¼ü

    virtual void keyMenuClicked();//Android ²Ëµ¥¼ü

private:
    HelloWorld *m_gameLayer;
    BackgroundLayer *m_backgroundLayer;
    GUILayer *m_GUILayer;
    

    WelcomeBoard *m_welcomeBoard;
    DeadEndBoard *m_deadEndBoard;
    GameState m_state;

    CCLabelAtlas *m_label;
};


#endif