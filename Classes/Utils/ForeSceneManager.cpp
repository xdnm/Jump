#include "ForeSceneManager.h"

ForeSceneManager::ForeSceneManager()
{
    m_unitHeight = CCDirector::sharedDirector()->getVisibleSize().height / 25;
    m_screenContent = new vector<B2CCNode*>();

    m_blockArray = CCArray::create();
    m_edgeArray  = CCArray::create();
    
    m_blockArray->retain();
    m_edgeArray->retain();

    m_screenSize = CCDirector::sharedDirector()->getVisibleSize();
    
    m_layer = NULL;
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

    for(int i = 0; i < m_screenContent->size(); i++)
    {
        CCNode *node = dynamic_cast<CCNode*>(m_screenContent->at(i));
        if(node != NULL)
            m_layer->removeChild(node);

    }
    m_screenContent->clear();
    
    
    //initBlockArray();
    //initEdgeArray();

    m_lastPillHeight = 0;
    this->schedule(schedule_selector(ForeSceneManager::generateBlocks1));
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
    pillManage(height);
    //edgeManage(height);
    //monstorManage(height);
}
void ForeSceneManager::generateBlocks(int layyerPositionY)
{
    blocksManage(-layyerPositionY);

    edgeManage(-layyerPositionY);

    //monstorManage(-layyerPositionY);
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
        /*Block *tempObject;
        for(int i = 0; i < 10; i++)
        {
            tempObject =(Block*)( m_blockArray->randomObject());

            if(tempObject->isAlive() == false)
                break;

            tempObject = NULL;
        }*/

        Block *tempObject;
        tempObject = getRandomBlockFromScene();



        if(tempObject != NULL)
        {
            int unitHeight = m_unitHeight + (maxHeight/8000) * 5;
            int xPosition = rand()%(int)(m_screenSize.width - tempObject->getB2NodeSize().width) + tempObject->getB2NodeSize().width / 2;
            int yPosition = maxHeight + (rand()%4 + 1)*unitHeight;
            //tempObject->setBlockPosition(ccp(xPosition, yPosition));
            tempObject->setAlive(true);
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
    int lastHeight = height;

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
       /* CCPoint point;
        point.x = rand()%((int)m_screenSize.width - 100) + 50;
        point.y = lastHeight + m_screenSize.height / 2 + rand()%((int)m_screenSize.height/2);
        Monster *monster = getRandomMonsterFromScene(point);
        m_screenContent->push_back(monster);*/
        Block *block = NULL;
        for(int i = m_screenContent->size() - 1; i > 0; --i)
        {
            if(TagHelper::Instance()->isObject(m_screenContent->at(i)->getTag(), ON_BLOCK))
            {
                block = dynamic_cast<Block*>(m_screenContent->at(i));
                break;
            }
        }

        if(block != NULL)
        {
            CCPoint point;
            point = block->getB2NodePostion();

            point.y += rand()%200 + 400;
            Monster *monster = getRandomMonsterFromScene(point);
            m_screenContent->push_back(monster);
        }
        
    }
}

void ForeSceneManager::pillManage(int height)
{
    int upperHeight = height + m_screenSize.height;
    if(upperHeight > m_lastPillHeight + 1000)
    {
        if(CCRANDOM_0_1() > 0.5f)
        {
            int typeNum = rand()%4;
            PillType type;
            switch (typeNum)
            {
            case 0:
                type = PT_RED;
                break;
            case 1:
                type = PT_BLUE;
                break;
            case 2:
                type = PT_YELLOW;
                break;
            case 3:
                type = PT_GREEN;
                break;
            default:
                type = PT_RED;
                break;
            }
            int width = m_screenSize.width;
            Pill *pill = Pill::createPillWithType(type, ccp(rand()%width, upperHeight));
            m_layer->addChild(pill);
            m_lastPillHeight = upperHeight;

        }
        else
        {
            m_lastPillHeight = m_lastPillHeight + 1000;
        }
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
    
    int presentheight = 0;
    if(m_layer != NULL)
        presentheight = -m_layer->getPositionY();

    if(ranNum < 80 - presentheight / 8000 )
        ranNum = rand()%80 + 1;
    else
        ranNum = rand()%20 + 81;

    if(ranNum > 100)
        ranNum = 100;

    Block *block = NULL;
    xml_node<> *blocksNode = m_presentScene->first_node("Blocks");

    //we had a random num(range from 1 to 100, then we iterate this scene's blocks node, find the first block which its value bigger than the random num)
    for(xml_attribute<> *eachBlock = blocksNode->first_attribute(); eachBlock != NULL; eachBlock = eachBlock->next_attribute())
    {
        if(atoi(eachBlock->value()) >= ranNum)
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
            else if(strcmp(blockNode->first_attribute("Class")->value(), "RubberBlock") == 0)
            {
                block = RubberBlock::createWithConfigNode(blockNode);
                //block = BrokenBlock::createBrokenBlock(blockNode);

                block->setAlive(false);
            }
            else if(strcmp(blockNode->first_attribute("Class")->value(), "MoveBlock") == 0)
            {
                block = MoveBlock::createMoveBlock(blockNode, 3.0f, 50);
                block->setAlive(false);
            }
            else if(strcmp(blockNode->first_attribute("Class")->value(), "BrokenBlock") == 0)
            {
                block = BrokenBlock::createBrokenBlock(blockNode);
                block->setAlive(false);
            }




            break;
        }
    }
    if(block == NULL)
    {
        return NULL;
    }
    return block;
}

Monster* ForeSceneManager::getRandomMonsterFromScene(CCPoint point)
{
    int ranNum = rand()%100 + 1;
    Monster *monter = NULL;
    xml_node<> *MonstersNode = m_presentScene->first_node("Monsters");
    
    //we had a random num(range from 1 to 100, then we iterate this scene's Monsters node, find the first Monster which its value bigger than the random num)
    for(xml_attribute<> *eachMonster = MonstersNode->first_attribute(); eachMonster != NULL; eachMonster = eachMonster->next_attribute())
    {
        if(atoi(eachMonster->value()) >= ranNum)
        {
            //this is the Monsternode we get from the scene.
            xml_node<> * MonsterNode = GameConfig::Instance()->getFirstMonsterNode(eachMonster->name());
            if(MonsterNode == NULL)
            {
                monter = NULL;
                break;
            }

            //in current design, each Monster may have different Class use different class to init.
            if(strcmp(MonsterNode->first_attribute("Class")->value(), "MonsterMushroom") == 0)
            {
                //Monster = RigidMonster::createRigidMonster(ccp(0, 0), CCSizeMake(50, 100), MonsterNode);
                monter = MonsterMushroom::createMushroomWithConfigNode(m_layer, point, MonsterNode);
                break;
            }

            break;
        }
    }

    return monter;
}