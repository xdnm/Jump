#include "cocos2d.h"
#include "Utils/GlobalData.h"
#include "Utils/TagHelper.h"
#include "Object/RigidBlock.h"
#include "Object/RigidEdge.h"
#include "Object/RubberBlock.h"
#include "Object/BrokenBlock.h"
#include "Object/MonsterMushroom.h"
#include  "Object/MoveBlock.h"
#include "Object/Pills.h"
#include "GameConfig.h"
USING_NS_CC;

class ForeSceneManager : CCNode
{
public:
    static ForeSceneManager* Instance()
    {
        static ForeSceneManager *instance = new ForeSceneManager();
        
        return instance;
    }

    void setWatchLayer(CCLayer *layer)
    {
        this->m_layer = layer;
        layer->addChild(this);
    }

    void generateBlocks1(float dt);

    void generateBlocks(int layyerPositionY);

    /**create a new Scene with a name, this scene could be config in Xmls/Scene.xml*/
    void createNewScene(char *SceneName);
    /**Delete all the contents in the pools, when we change the forescene we can use this function*/
    void deleteAll();

    Block* getRandomBlockFromScene();
    Monster* getRandomMonsterFromScene(CCPoint point);


private:
    ForeSceneManager();

    void initBlockArray();

    void initEdgeArray();

    void blocksManage(int height);
    void edgeManage(int height);
    void monstorManage(int height);
    void pillManage(int height);

    B2CCNode* getTopObject(int tag);
    B2CCNode* getBottomObject(int tag);

    B2CCNode* nameProject(char *name);
private:
    CCLayer *m_layer;
    CCArray *m_blockArray;
    CCArray *m_edgeArray;
    CCSize m_screenSize;

    rapidxml::file<> *m_file;
    rapidxml::xml_document<> m_doc;
    rapidxml::xml_node<> *m_presentScene;

    vector<B2CCNode*> *m_screenContent;
    int m_unitHeight;

    int m_lastPillHeight;
};