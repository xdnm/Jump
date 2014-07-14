#ifndef __DEADEND_BOARD__
#define __DEADEND_BOARD__

#include "cocos2d.h"
USING_NS_CC;

class DeadEndBoard : public CCSprite
{
public:
    ~DeadEndBoard();

    static DeadEndBoard* createDeadEndBoard(int score);
    
    bool initDeadEndBoard(int score);

    void loadShader();

    virtual void draw();

    void update(float dt);
protected:
    DeadEndBoard();
private:
    GLuint m_utexture, m_utime, m_uresolution;
    GLfloat m_time;

    GLuint m_texture;

    CCRect m_screenRect;

    ccBlendFunc m_blendFunc;
};
#endif