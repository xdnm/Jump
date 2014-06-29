#ifndef _MONSTER_MUSHROOM__
#define  _MONSTER_MUSHROOM__

#include "Object/Monster.h"

class MonsterMushroom : public Monster
{
public:
    ~MonsterMushroom();

    bool initModel();

    static MonsterMushroom *createMushroom(CCLayer *layer, CCPoint position, CCSize size, void *parm);
    static MonsterMushroom *createMushroomWithConfigNode(CCLayer *layer, CCPoint position,  xml_node<> *node);

    bool initMushroom(CCLayer *layer, CCPoint position, CCSize size, void *parm);
    bool initMushroomWithConfigNode(CCLayer *layer,CCPoint position,  xml_node<> *node);

    bool attacked(ObjectModel *model);
    bool beenTrampled(ObjectModel *model);
    bool beenAttacked(ObjectModel *model);
    
    void draw();

    bool isReady();
protected:
private:
    MonsterMushroom();

    bool createBody();

private:
    b2Body *m_bottomBody;
    b2Body *m_topBody;

    //render data;
    Vertex2D m_vertexCoords[4];
    Vertex2D m_textureCoords[4];

    CCTexture2D *m_texture;
};

#endif