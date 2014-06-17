#include "GameConfig.h"

GameConfig::GameConfig()
{
    string filePath = CCFileUtils::sharedFileUtils()->getWritablePath();

    string tempPath = filePath + "Scene.xml";
    m_scenefile = new rapidxml::file<>(tempPath.c_str());
    m_sceneDoc.parse<0>(m_scenefile->data());

    tempPath = filePath + "Monster.xml";
    m_monsterFile = new rapidxml::file<>(tempPath.c_str());
    m_monsterDoc.parse<0>(m_monsterFile->data());

    tempPath = filePath + "Block.xml";
    m_blockFile = new rapidxml::file<>(tempPath.c_str());
    m_blockDoc.parse<0>(m_blockFile->data());
}

GameConfig::~GameConfig()
{
    SAFE_DELETE(m_scenefile);
    SAFE_DELETE(m_monsterFile);
    SAFE_DELETE(m_blockFile);

}

GameConfig* GameConfig::Instance()
{
    static GameConfig* instance = new GameConfig();

    return instance;
}

xml_node<>* GameConfig::getFirstSceneNode(char *name /* = 0 */)
{
    xml_node<> *tempNode = m_sceneDoc.first_node();
    xml_node<> *node;
    if(name == 0)
    {
        node = tempNode->first_node();
    }
    else
    {
        for(node = tempNode->first_node(); node != NULL; node = node->next_sibling())
        {
            if(strcmp(node->first_attribute("Name")->value(), name) == 0)
                break;
        }
    }

    return node;
}

xml_node<>* GameConfig::getFirstMonterNode(char *name /* = 0 */)
{
    xml_node<>* tempNode = m_monsterDoc.first_node();
    xml_node<> *node;
    
    node = tempNode->first_node(name);

    return node;
}

xml_node<>* GameConfig::getFirstBlockNode(char *name /* = 0 */)
{
    xml_node<> *node = m_blockDoc.first_node()->first_node(name);

    return node;
}
