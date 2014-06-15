#include "cocos2d.h"
#include "Utils/GlobalData.h"
#include "Utils/TagHelper.h"
#include "Object/RigidBlock.h"
#include "Object/RigidEdge.h"
#include "Utils/RapidXml.h"
USING_NS_CC;

class ForeSceneManager : CCObject
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
    }


    void generateBlocks(int layyerPositionY);

    /**create a new Scene with a name, this scene could be config in Xmls/Scene.xml*/
    void createNewScene(char *SceneName);
    /**Delete all the contents in the pools, when we change the forescene we can use this function*/
    void deleteAll();

private:
    ForeSceneManager();

    void initBlockArray();

    void initEdgeArray();

    void blocksManage(int height);
    void edgeManage(int height);
    void monstorManage(int height);

    B2CCNode* getTopObject(int tag);
    B2CCNode* getBottomObject(int tag);

    B2CCNode* nameProject(char *name);
private:
    CCLayer *m_layer;
    CCArray *m_blockArray;
    CCArray *m_edgeArray;
    CCSize m_screenSize;


    vector<B2CCNode*> *m_screenContent;
    int m_unitHeight;
};