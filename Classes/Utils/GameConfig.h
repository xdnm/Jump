#ifndef __GAME_CONFIG__
#define __GAME_CONFIG__


#include "cocos2d.h"
#include "Utils/RapidXml.h"
#include "Utils/GlobalData.h"
USING_NS_CC;

//class XmlNode
//{
//public:
//    XmlNode(xml_node<> *node)
//    {
//        this->m_node = node;
//    }
//
//    XmlNode getFirstNode(char *ch = 0)
//    {
//        return XmlNode(m_node->first_node(ch));
//    }
//private:
//    xml_node<> *m_node;
//};

class GameConfig
{
public:
    ~GameConfig();
    static GameConfig* Instance();

    xml_node<> *getFirstSceneNode(char *name = 0);

    xml_node<> *getFirstMonterNode(char *name = 0);

    xml_node<> *getFirstBlockNode(char *name = 0);

private:
    GameConfig();

private:
    rapidxml::file<> *m_scenefile;
    rapidxml::xml_document<> m_sceneDoc;

    rapidxml::file<> *m_monsterFile;
    rapidxml::xml_document<> m_monsterDoc;

    rapidxml::file<> *m_blockFile;
    rapidxml::xml_document<> m_blockDoc;
};
#endif