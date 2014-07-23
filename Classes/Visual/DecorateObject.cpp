#include "DecorateObject.h"

DecorateObject::DecorateObject()
{
    
}

DecorateObject::~DecorateObject()
{

}

DecorateObject* DecorateObject::createDecObject(const char* filename, int distance, CCLayer *layer)
{
    DecorateObject *pRet = new DecorateObject();

    if(pRet != NULL && pRet->initDecobject(filename, distance, layer))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool DecorateObject::initDecobject(const char*filename, int distance,  CCLayer *layer)
{
    m_distance = distance;
    m_layer = layer;
    m_layerInitHeight = m_layer->getPositionY();

    this->initWithFile(filename);

    this->schedule(schedule_selector(DecorateObject::lifeUpdate), 0.2);

    return true;
}

void DecorateObject::lifeUpdate(float dt)
{
    CCPoint point = this->convertToWorldSpace(ccp(0, 0));

    if(point.y < -400)
    {
        //this->removeFromParent();
        
    }
    else
    {
        int offset = (m_layer->getPositionY() - m_layerInitHeight) / m_distance;

        if(offset < 0)
        {
            this->setPositionY(this->getPositionY() + offset);
            m_layerInitHeight = m_layer->getPositionY();
        }
    }
}
