#include "DeadEndBoard.h"

DeadEndBoard::DeadEndBoard()
{

}

DeadEndBoard::~DeadEndBoard()
{
    this->removeAllChildren();
}

DeadEndBoard* DeadEndBoard::createDeadEndBoard(int score)
{
    DeadEndBoard *pRet = new DeadEndBoard();

    if(pRet != NULL && pRet->initDeadEndBoard(score))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return pRet;
    }
}

bool DeadEndBoard::initDeadEndBoard(int score)
{
     CCSize size = CCDirector::sharedDirector()->getVisibleSize();
    m_screenRect = CCEGLView::sharedOpenGLView()->getViewPortRect();
    this->init();
    this->setAnchorPoint(ccp(0.5f, 0.5f));
    this->setPosition(ccp(size.width/2, size.height/2));
    this->setContentSize(size);
   

    CCLabelAtlas *label = CCLabelAtlas::create("0", "fonts/Text2.plist");
    label->setAnchorPoint(ccp(0.5f, 0.5f));
    label->setScale((size.height/15) / label->getContentSize().height);
    //label->setColor(ccRED);
    label->setPosition(this->getContentSize().width/2, this->getContentSize().height/2);

    char s[20];
    sprintf(s, "SCORE", score);

    label->setString(s);

    this->addChild(label);

    CCLabelAtlas *scoreLabel = CCLabelAtlas::create("0", "fonts/Text2.plist");
    scoreLabel->setAnchorPoint(ccp(0.5f, 0.5f));
    scoreLabel->setScale((size.height/15) / scoreLabel->getContentSize().height);
    scoreLabel->setPosition(this->getContentSize().width/2, this->getContentSize().height/2 - label->getContentSize().height * 1.2f);
    memset(s, 0, 20);
    sprintf(s, "%d", score);
    scoreLabel->setString(s);

    this->addChild(scoreLabel);
    
    this->scheduleUpdate();
    m_texture = CCTextureCache::sharedTextureCache()->addImage("transparent.png")->getName();
    CCLog("GL_COPY : %f, %f, %f, %f", m_screenRect.origin.x, m_screenRect.origin.y, m_screenRect.size.width, m_screenRect.size.height);
    ccGLBindTexture2D(m_texture);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_screenRect.origin.x, m_screenRect.origin.y, m_screenRect.size.width, m_screenRect.size.height, 0);

    loadShader();

    return true;
}

void DeadEndBoard::loadShader()
{
    CCGLProgram *shader = new CCGLProgram();

    shader->initWithVertexShaderFilename("gray.vsh", "gray.fsh");

    shader->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
    shader->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);

    shader->link();

    shader->updateUniforms();

    m_utexture = glGetUniformLocation(shader->getProgram(), "tex0");
    m_utime     = glGetUniformLocation(shader->getProgram(), "time");
    m_uresolution = glGetUniformLocation(shader->getProgram(), "resolution");

    this->setShaderProgram(shader);
    shader->release();

    m_blendFunc.src = GL_SRC_ALPHA;
    m_blendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
    m_time = 0;
}

void DeadEndBoard::draw()
{
    CC_NODE_DRAW_SETUP();
    CCSize size = CCDirector::sharedDirector()->getVisibleSize();
    glBlendFunc(m_blendFunc.src, m_blendFunc.dst);
    float w = size.width;
    float h = size.height;

    CCGLProgram *shader = getShaderProgram();
    shader->setUniformLocationWith1f(m_utime, m_time);
    shader->setUniformLocationWith1i(m_utexture, 0);
    shader->setUniformLocationWith2f(m_uresolution, w, h);
    
    CCLog("GL_COPY : %f, %f, %f, %f", m_screenRect.origin.x, m_screenRect.origin.y, m_screenRect.size.width, m_screenRect.size.height);
    ccGLBindTexture2D(m_texture);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_screenRect.origin.x, m_screenRect.origin.y, m_screenRect.size.width, m_screenRect.size.height, 0);
    
    glEnableVertexAttribArray(kCCVertexAttrib_Position);
    glEnableVertexAttribArray(kCCVertexAttrib_TexCoords);

    GLfloat vertices[12] = {  
        0, 0, //左下0
        w, 0, //右下1
        w, h, //右上2
        0, 0, //左下0
        0, h, //左上3
        w, h, //右上2
    };  

    GLfloat texcoords[12] = {
        0, 0,
        1, 0,
        1, 1,
        0, 0,
        0, 1,
        1, 1,
    };

    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, texcoords);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void DeadEndBoard::update(float dt)
{
    m_time += dt;
}