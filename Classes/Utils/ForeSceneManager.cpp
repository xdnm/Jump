#include "ForeSceneManager.h"

ForeSceneManager::ForeSceneManager()
{
    m_unitHeight = CCDirector::sharedDirector()->getVisibleSize().height / 2;
    m_screenContent = new vector<B2CCNode*>();

    m_blockArray = CCArray::create();
    m_edgeArray  = CCArray::create();
    
    m_blockArray->retain();
    m_edgeArray->retain();

    m_screenSize = CCDirector::sharedDirector()->getVisibleSize();

    initBlockArray();
    initEdgeArray();
}

void ForeSceneManager::initBlockArray()
{
    for(int i = 0; i < 20; i++)
    {
        RigidBlock *tempBlock = RigidBlock::createRigidBlock(ccp(0, 0),CCSizeMake(50, 20), NULL);
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
            tempObject->setBlockPosition(ccp(xPosition, yPosition));
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
        maxHeight = height - Edge::s_height/2;
    else
        maxHeight = (*iter)->getB2NodePostion().y;

    if(maxHeight < height + Edge::s_height/2)
    {
        RigidEdge* leftedge = RigidEdge::createRigidEdge(ccp(RigidEdge::s_leftX, maxHeight +Edge::s_height + 10), Edge::s_height, NULL);
        RigidEdge* rightEdge = RigidEdge::createRigidEdge(ccp(RigidEdge::s_rightX, maxHeight +Edge::s_height + 10), Edge::s_height, NULL);
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