#include "ForeSceneManager.h"

ForeSceneManager::ForeSceneManager()
{
    m_unitHeight = CCDirector::sharedDirector()->getVisibleSize().height / 15;
    m_screenContent = new vector<B2CCNode*>();

    m_blockArray = CCArray::create();
    m_edgeArray  = CCArray::create();
    
    m_blockArray->retain();
    m_edgeArray->retain();

    m_screenSize = CCDirector::sharedDirector()->getVisibleSize();
    
}

void ForeSceneManager::createNewScene(char *SceneName)
{
    m_presentScene = GameConfig::Instance()->getFirstSceneNode("BoardLand");

    if(m_presentScene == NULL)
    {
        CCLOG("ReadScene config failed!!!Please Check the Scene.xml file");
        return;
    }
    else
        CCLOG("Begin to build the Scene: %s.", m_presentScene->first_attribute("Name"));

    m_blockArray->removeAllObjects();
    m_edgeArray->removeAllObjects();
    
    m_screenContent->clear();

    initBlockArray();
    initEdgeArray();

    this->schedule(schedule_selector(ForeSceneManager::generateBlocks1), 0.1f);
}
void ForeSceneManager::deleteAll()
{
    SAFE_DELETE(m_screenContent);
    m_blockArray->release();
    m_edgeArray->release();
}

void ForeSceneManager::initBlockArray()
{
    for(int i = 0; i < 20; i++)
    {
        //RigidBlock *tempBlock = RigidBlock::createRigidBlock(ccp(0, 0),CCSizeMake(120, 40), NULL);
        Block* block = getRandomBlockFromScene();
        block->setAlive(false);
        //tempBlock->autorelease();
        //tempBlock->retain();
        m_blockArray->addObject(block);
    }

    for(int i = 0; i < 20; i++)
    {
        RigidBlock *tempBlock = (RigidBlock *)(m_blockArray->objectAtIndex(i));
        tempBlock->setAlive(false);
    }
}

void ForeSceneManager::initEdgeArray()
{

}

void ForeSceneManager::generateBlocks1(float dt)
{
    int height = -m_layer->getPositionY();
    blocksManage(height);
    edgeManage(height);
    monstorManage(height);
}
void ForeSceneManager::generateBlocks(int layyerPositionY)
{
    blocksManage(-layyerPositionY);

    edgeManage(-layyerPositionY);

    monstorManage(-layyerPositionY);
}

void ForeSceneManager::blocksManage(int height)
{
    int screenTop = height + m_screenSize.height;

   // xml_node<> *node = m_file.first_node();

    Block* lastBlock = NULL;
    xml_node<> *node = m_doc.first_node();
    for(int i = m_screenContent->size() - 1; i >= 0; i--)
    {
        int tag = m_screenContent->at(i)->getTag();
        if(TagHelper::Instance()->isObject(tag, ON_BLOCK))
        {
            lastBlock = (Block*)(m_screenContent->at(i));
            break;
        }
    }

    int maxHeight;
    if(lastBlock != NULL)
    {
        maxHeight = lastBlock->getB2NodePostion().y;
    }
    else
    {
        maxHeight = height;   
    }

    if(maxHeight < screenTop)
    {
        //try to get the the block from block buffer 10 times
        Block *tempObject;
        for(int i = 0; i < 10; i++)
        {
            tempObject =(Block*)( m_blockArray->randomObject());

            if(tempObject->isAlive() == false)
                break;

            tempObject = NULL;
        }

        if(tempObject != NULL)
        {
            int xPosition = rand()%(int)(m_screenSize.width - tempObject->getB2NodeSize().width) + tempObject->getB2NodeSize().width / 2;
            int yPosition = maxHeight + (rand()%4 + 1)*m_unitHeight;
            //tempObject->setBlockPosition(ccp(xPosition, yPosition));
            tempObject->setB2NodePostion(ccp(xPosition, yPosition));
            m_layer->addChild((CCNode*)tempObject);

            m_screenContent->push_back(tempObject);
        }
    }
    //end add blocks in the top

    Block *bottomBlock = NULL;
    vector<B2CCNode*>::iterator iter;
    for(iter = m_screenContent->begin(); iter != m_screenContent->end(); )
    {
        if(TagHelper::Instance()->isObject((*iter)->getTag(), ON_BLOCK))
        {
            if((*iter)->getB2NodePostion().y < height)
            {
                (*iter)->setAlive(false);
                m_layer->removeChild((CCNode*)(*iter));
                m_screenContent->erase(iter);
               
            }
            break;
        }

        iter++;
    }


}

void ForeSceneManager::edgeManage(int height)
{
    vector<B2CCNode*>::iterator iter = m_screenContent->end();
    iter--;
    for(; iter != m_screenContent->begin(); )
    {
        if(TagHelper::Instance()->isObject((*iter)->getTag(), ON_EDGE))
        {
            break;
        }
        iter--;
    }

    int maxHeight;
    if(iter == m_screenContent->begin())
        maxHeight = height - m_screenSize.height/2;
    else
        maxHeight = (*iter)->getB2NodePostion().y;

    if(maxHeight < height + m_screenSize.height/2)
    {
        RigidEdge* leftedge = RigidEdge::createRigidEdge(ccp(0, maxHeight +m_screenSize.height), m_screenSize.height, NULL);
        RigidEdge* rightEdge = RigidEdge::createRigidEdge(ccp(m_screenSize.width, maxHeight +m_screenSize.height), m_screenSize.height, NULL);
        m_screenContent->push_back(leftedge);
        m_screenContent->push_back(rightEdge);
        leftedge->retain();
        rightEdge->retain();
    }
    
    
    for(iter = m_screenContent->begin(); iter != m_screenContent->end(); )
    {
        if(TagHelper::Instance()->isObject((*iter)->getTag(), ON_EDGE) && (*iter)->getB2NodePostion().y < height - m_screenSize.height / 2)
        {
            (*iter)->autorelease();
            iter = m_screenContent->erase(iter);
        }
        else
            iter++;
    }
}

void ForeSceneManager::monstorManage(int height)
{
    int lastHeight = 0;

    //Delete the monsters which is below the screen;
    vector<B2CCNode*>::iterator iter;
    for(iter = m_screenContent->begin(); iter != m_screenContent->end(); )
    {
        if(TagHelper::Instance()->isObject((*iter)->getTag(), ON_MONSTOR))
        {
            lastHeight = (*iter)->getB2NodePostion().y;

            if((*iter)->getB2NodePostion().y < height)
            {
                (*iter)->setAlive(false);
                m_layer->removeChild((CCNode*)(*iter));
                iter = m_screenContent->erase(iter);
            }
            else
                iter++;

        }
        else
            iter++;
    }

    if(height + m_screenSize.height > lastHeight)
    {

    }
}

B2CCNode *ForeSceneManager::nameProject(char *name)
{
    B2CCNode *pRet = NULL;
    do 
    {
        if(strcmp(name, "Mushroom"))
        {
            //pRet = MonsterMushroom::createMushroom(m_layer, ccp())
            break;
        }
        else if(strcmp(name, "RigidBlock"))
        {
            //pRet = RigidBlock::createRigidBlock(ccp(0, 0), )
        }
    } while (0);

    return pRet;
}

Block* ForeSceneManager::getRandomBlockFromScene()
{
    int ranNum = rand()%100 + 1;
    Block *block = NULL;
    xml_node<> *blocksNode = m_presentScene->first_node("Blocks");

    //we had a random num(range from 1 to 100, then we iterate this scene's blocks node, find the first block which its value bigger than the random num)
    for(xml_attribute<> *eachBlock = blocksNode->first_attribute(); eachBlock != NULL; eachBlock = eachBlock->next_attribute())
    {
        if(atoi(eachBlock->value()) > ranNum)
        {
            //this is the blocknode we get from the scene.
            xml_node<> * blockNode = GameConfig::Instance()->getFirstBlockNode(eachBlock->name());
            if(blockNode == NULL)
            {
                block = NULL;
                break;
            }

            //in current design, each block may have different Class use different class to init.
            if(strcmp(blockNode->first_attribute("Class")->value(), "RigidBlock") == 0)
            {
                //block = RigidBlock::createRigidBlock(ccp(0, 0), CCSizeMake(50, 100), blockNode);
               block = RigidBlock::createWithConfigNode(blockNode);
            }

             break;
        }
    }

    return block;
}