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

    rapidxml::file<> fdoc(CCFileUtils::sharedFileUtils()->fullPathForFilename("Xmls/Scene.xml").c_str());
    rapidxml::xml_document<> SceneDoc;
    SceneDoc.parse<0>(fdoc.data());

    xml_node<> *node = SceneDoc.first_node();
    xml_node<> *node1 = node->first_node();
    xml_attribute<char> *attr = node1->first_attribute("name");
    attr->value();
    
    initBlockArray();
    initEdgeArray();
}

void ForeSceneManager::createNewScene(char *SceneName)
{
    
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
        RigidBlock *tempBlock = RigidBlock::createRigidBlock(ccp(0, 0),CCSizeMake(120, 40), NULL);
        tempBlock->setAlive(false);
        //tempBlock->autorelease();
        //tempBlock->retain();
        m_blockArray->addObject(tempBlock);
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

void ForeSceneManager::generateBlocks(int layyerPositionY)
{
    blocksManage(-layyerPositionY);

    edgeManage(-layyerPositionY);

    monstorManage(-layyerPositionY);
}

void ForeSceneManager::blocksManage(int height)
{
    //B2CCNode *tempObject = (B2CCNode *)(m_blockArray->randomObject());
    //m_layer->addChild(tempObject);
    //tempObject->release();
    //tempObject->release();

    //add new block in the top
    int screenTop = height + m_screenSize.height;

    Block* lastBlock = NULL;

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
        if(TagHelper::Instance()->isObject((*iter)->getTag(), ON_EDGE) && (*iter)->getB2NodePostion().y < height - Edge::s_height / 2)
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