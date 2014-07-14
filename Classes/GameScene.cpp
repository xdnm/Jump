#include "GameScene.h"

GameScene::GameScene()
{
    welcome();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);

}

GameScene::~GameScene()
{

}

GameScene* GameScene::Instance()
{
    static GameScene* instance = NULL;

    if(instance == NULL)
        instance = new GameScene();

    return instance;
}

CCScene* GameScene::scene()
{
    static CCScene *gameScene = CCScene::create();

    GameScene *instance = new GameScene();

    gameScene->addChild(instance);

    return gameScene;
}

void GameScene::CreateNewScene(void *parm)
{
    m_gameLayer = HelloWorld::create();
    m_backgroundLayer = BackgroundLayer::createBackground(this,m_gameLayer);
    m_GUILayer = GUILayer::createGUILayerWithScene(this, m_gameLayer);

    this->addChild(m_gameLayer, 3);
    m_GUILayer->setZOrder(4);
    m_backgroundLayer->setZOrder(1);
    this->schedule(schedule_selector(GameScene::gameUpdate), 0.1f);

    this->setGameState(GS_STOP);
}

void GameScene::gameUpdate(float dt)
{
    if(m_gameLayer->GetRole()->getB2NodePostion().y < -(m_gameLayer->getPositionY()) - 100)
    {
        CCLog("Role dead!");

   
        if(m_state == GS_ONGOING)
            roleDead();

     
    }
}

void GameScene::setGameState(GameState state)
{
    m_state = state;
    m_gameLayer->setGameState(state);
}

void GameScene::welcome()
{
    CreateNewScene(NULL);
    this->setGameState(GS_NEWGAME);

    m_welcomeBoard = WelcomeBoard::createWelcomeBoard();
    CCSize size = CCDirector::sharedDirector()->getVisibleSize();
    m_welcomeBoard->setPosition(ccp(size.width/2.0f, size.height * 0.75f));

    this->addChild((CCNode *)m_welcomeBoard, 10);
}

bool GameScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if(m_state == GS_NEWGAME)
    {
        if(m_welcomeBoard != NULL)
        {
            m_welcomeBoard->removeFromParent();
            setGameState(GS_ONGOING);

        }
    }
    else if(m_state == GS_STOP)
    {
        setGameState(GS_ONGOING);
    }
    else if(m_state == GS_DEADSTOP)
    {
        if(m_GUILayer->isDeadBoard())
        {
            m_gameLayer->reset();
            m_backgroundLayer->reset();
            m_GUILayer->reset();
            setGameState(GS_ONGOING);
        }
    }

    return true;
}

void GameScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{

}

void GameScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{

}

void GameScene::roleDead()
{
    m_GUILayer->runDeadBoard();

    this->setGameState(GS_DEADSTOP);
}