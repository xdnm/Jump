#include "Object/Monster.h"

class MonsterMushroom : public Monster
{
public:
    ~MonsterMushroom();

    bool initModel();

    static MonsterMushroom *createMushroom(CCLayer *layer, CCPoint position, CCSize size, void *parm);

    bool initMushroom(CCLayer *layer, CCPoint position, CCSize size, void *parm);

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