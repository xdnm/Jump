#include "MyLabel.h"

MyLabel::MyLabel()
{

}

MyLabel::~MyLabel()
{

}

CCLabelAtlas* MyLabel::createMyLabel(const char* str)
{
    CCLabelAtlas *label = CCLabelAtlas::create(str, "fonts/Text2.plist");

    label->setAnchorPoint(ccp(0.5f, 0.5f));
    label->setScale((CCDirector::sharedDirector()->getVisibleSize().height/15) / label->getContentSize().height);
    //label->setPosition(this->getContentSize().width/2, this->getContentSize().height/2 - 2 * label->getContentSize().height * 1.2f);

    return label;
}

bool MyLabel::initMyLabel(const char* str)
{
   return true;
}
