#include "HelloWorldScene.h"
#include "Object/RigidEdge.h"
#include "Object/MonsterMushroom.h"
USING_NS_CC;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    BackgroundLayer *background = BackgroundLayer::createBackground(scene, layer);
	GUILayer              *gui = GUILayer::createGUILayerWithScene(scene, layer);
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    CCLog("copy file..");
    copyData("Scene.xml");
    copyData("Block.xml");
    copyData("Monster.xml");
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.


    m_role = NULL;


	//--------------init world-------------------------
	b2Vec2 gravity = b2Vec2(0.0f, -40.0f);
	bool doSleep = true;
	//world = new b2World()
	m_world = new b2World(gravity);
	m_world->SetAllowSleeping(true);
    //GLESDebugDraw *debugDrawFlag;
    //debugDrawFlag = new GLESDebugDraw(PTM_RATIO);
    //m_world->SetDebugDraw(debugDrawFlag);
    //uint32	flags = 0;
    //flags += b2Draw::e_shapeBit;
    //flags += b2Draw::e_jointBit;
    //debugDrawFlag->SetFlags(flags);
	B2Helper::Instance()->setWorld(m_world);

    //--------------end init world----------------------

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0, 0);

	b2Body *groundBody = m_world->CreateBody(&groundBodyDef);

	b2EdgeShape groundBox;
	b2FixtureDef boxShapeDef;
	boxShapeDef.filter.groupIndex = 1;
	boxShapeDef.shape = &groundBox;
	groundBox.Set(b2Vec2(0, 0), b2Vec2(visibleSize.width/PTM_RATIO, 0));

	groundBody->CreateFixture(&boxShapeDef);

	groundBox.Set(b2Vec2(0, 0), b2Vec2(0, visibleSize.height/PTM_RATIO));
	//groundBody->CreateFixture(&boxShapeDef);

	groundBox.Set(b2Vec2(0, visibleSize.height/PTM_RATIO), b2Vec2(visibleSize.width/PTM_RATIO, visibleSize.height/PTM_RATIO));
	//groundBody->CreateFixture(&boxShapeDef);

	groundBox.Set(b2Vec2(visibleSize.width/PTM_RATIO, visibleSize.height/PTM_RATIO), b2Vec2(visibleSize.width/PTM_RATIO, 0));
	//groundBody->CreateFixture(&boxShapeDef);

	//test block---------------------------------------------------
	b2PolygonShape polyShape;
	
	//end test block---------------------------------------------



    //--------------test region-------------------------
	
    /*int height = 0;
    for(int i = 0; i < 100; i++)
    {
        CCSprite *tempSprite = CCSprite::create("CloseNormal.png");
        tempSprite->setPosition(ccp(150, height));
        this->addChild(tempSprite);
        height += 100;
    }
    
    MonsterMushroom *mush = MonsterMushroom::createMushroom(this, ccp(200, 200), CCSizeMake(100, 100), NULL);
    this->addChild((CCNode*)mush);
    
    m_streak = MyCCMotionStreak::create(0.5f, 3.0f, 10, ccWHITE, "CloseNormal.png");
    m_streak->setFollowNode(m_role, m_role->m_weapon, ccp(50, 200));*/

    //m_guiLayer = GUILayer::createGUILayer(this);

    //this->addChild(m_streak);
    
    //body->ApplyForce(b2Vec2(10.0f, 0), body->GetWorldCenter());
    //body->ApplyForce(b2Vec2(0, body->GetMass() * gravity.Length()) , body->GetWorldCenter());

    createNewGame();

    Pill *tempPill = Pill::createPillWithType(PT_RED, ccp(CCDirector::sharedDirector()->getVisibleSize().width/2, 500));
    this->addChild(tempPill);
	//--------------end test region--------------------


	//--------------Register region--------------------------
    ForeSceneManager::Instance()->setWatchLayer(this);
    //ForeSceneManager::Instance()->createNewScene("PromiseLand");
    B2Handler::createB2Handler(m_world);
	this->schedule(schedule_selector(HelloWorld::worldTick));
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    this->setAccelerometerEnabled(true);
    
	//--------------end register region---------------------
    return true;
}

void HelloWorld::draw()
{
	CCLayer::draw();

	ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position);

	kmGLPushMatrix();
	m_world->DrawDebugData();
	kmGLPopMatrix();
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}

void HelloWorld::worldTick(float dt)
{
    if(m_state != GS_ONGOING)
        return;

    B2Helper::Instance()->bodiesListener(dt);
	m_world->Step(dt, 8, 8);
    
}

bool HelloWorld::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	//m_role->jump(10.0f);
    CCPoint touchPoint = this->convertTouchToNodeSpace(pTouch);
    CCLog("Touch began. Screen point : ( %f, %f)", touchPoint.x, touchPoint.y);
    m_role->onTouchBegan(pTouch, pEvent);

	return true;
}
void HelloWorld::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchPoint = this->convertTouchToNodeSpace(pTouch);
   // CCLog("Touch Ended. Screen point : ( %f, %f)", touchPoint.x, touchPoint.y);
    m_role->onTouchEnded(pTouch, pEvent);
}
void HelloWorld::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchPoint = this->convertTouchToNodeSpace(pTouch);
    //CCLog("Touch Moved. Screen point : ( %f, %f)", touchPoint.x, touchPoint.y);
    //m_streak->setPosition(touchPoint);
    m_role->onTouchMoved(pTouch, pEvent);
}
void HelloWorld::didAccelerate(CCAcceleration* pAccelerationValue)
{
   /* b2Vec2 gravity (pAccelerationValue->x * 100, pAccelerationValue->y * 100);
    m_world->SetGravity(gravity);*/
   
    m_role->setAcceleration(pAccelerationValue);
    //CCLog("Current acceleration is : (%f, %f).", pAccelerationValue->x, pAccelerationValue->y );
}


void HelloWorld::copyData(const char* pFileName)
{
    std::string strPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(pFileName);
    unsigned long len = 0;
    unsigned char *data = NULL;
    
    if(isFileExist(pFileName) == false)
    {
        CCLog("File %s does not exsit.", pFileName);
    }
    
    data = CCFileUtils::sharedFileUtils()->getFileData(strPath.c_str(),"r",&len);
    if(len == 0)
    {
        CCLog("File %s does not exsit!", pFileName);
    }
    
    std::string destPath = CCFileUtils::sharedFileUtils()->getWritablePath();
    destPath += pFileName;
    CCLog("copy file to %s", destPath.c_str());
    FILE *fp = fopen(destPath.c_str(),"w+");
    fwrite(data,sizeof(char),len,fp);
    fclose(fp);
    delete []data;
    data = NULL;
}

void HelloWorld::createWelcome()
{

}

void HelloWorld::createNewGame()
{
    //--------------set role----------------------------
   

    m_role = RoleObject::CreateRole(m_world, NULL);
    //m_role->setWorldPosition(ccp(300, 200));
    m_role->resetRole();
    this->addChild(m_role, 10);
    this->stopAllActions();
    this->runAction(JumpFollow::create(m_role->m_visiableNode));
    ForeSceneManager::Instance()->createNewScene("PromiseLand");

    
    this->setPositionY(0.0f);

    this->setGameState(GS_ONGOING);
    //
    
    
    
    //--------------end set role------------------------

}

void HelloWorld::reset()
{
    if(m_role != NULL && m_role->getParent() != NULL)
    {
        m_role->resetRole();
        this->setPositionY(0.0f);
        m_role->update(1.0f);
        ForeSceneManager::Instance()->createNewScene("PromiseLand");
        return;
    }
}

void HelloWorld::setGameState(GameState state)
{
    m_state = state;
    if(m_state == GS_ONGOING)
    {
        
    }
}