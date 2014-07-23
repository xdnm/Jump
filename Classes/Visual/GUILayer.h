#ifndef __GUI_LAYER__
#define __GUI_LAYER__

#include "cocos2d.h"
#include "Visual/ScoreBoard.h"
#include "Visual/DeadEndBoard.h"
#include "Visual/PowerColumn.h"

USING_NS_CC;

class GUILayer : public cocos2d::CCLayer
{
public:
    ~GUILayer();
    static GUILayer* createGUILayer(CCLayer *gameLayer);
    static GUILayer* Instance();
    bool initWithGameLayer(CCLayer *gameLayer);

    static GUILayer* createGUILayerWithScene(CCNode *scene, CCLayer *layer);
    bool initWithGameLayerWithScene(CCNode *scene, CCLayer *layer);

    void runDeadBoard();
    void stopDeadBoard();
    bool isDeadBoard();

    void reset();

    void onRoleJumping();
    void onRoleHooking();

    int getRolePower();
    void clearRolePower();

    void textAnimationEnd(CCNode *node);

    void addBouns(int bouns);

    void runBounsAction();

    void decreaseScoreRatio();

    void addPowerUnit(int parm = 0);
protected:


    GUILayer();



private:
    CCLayer *m_gameLayer;

    ScoreBoard *m_scoreBoard;
    DeadEndBoard *m_deadEndBoard;
    PowerColumn *m_powerColum;

    static GUILayer *instance;

    int m_hookCount;

    int m_powerUnit;
};

#endif