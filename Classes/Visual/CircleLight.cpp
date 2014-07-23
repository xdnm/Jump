#include "CircleLight.h"

CircleLight::CircleLight()
{

}

CircleLight *CircleLight::createCircleLight(int innerRadius, int radius)
{
    CircleLight *circel = new CircleLight();
    if(circel && circel->initWithRadius(innerRadius, radius))
    {
        circel->autorelease();
        return circel;
    }
    else
    {
        CC_SAFE_DELETE(circel);
        return NULL;
    }
}

//如参数名所示，我们的光晕由以下参数生成：光晕内直径，外直径，闪烁周期，alpha参数，颜色
//其中alpha参数传递给shader使用，可以调控闪烁的强度
CircleLight *CircleLight::createCircleLight(int innerRadius, int radius, float timeCircle, float alphaParm, ccColor4F color)
{
    CircleLight* light = new CircleLight();

    if(light != NULL && light->initWithAllParm(innerRadius, radius, timeCircle, alphaParm, color))
    {
        light->autorelease();
        return light;
    }
    else
    {
        CC_SAFE_DELETE(light);
        return NULL;
    }
}

bool CircleLight::initWithAllParm(int innerRadius, int radius, float timeCircle, float alphaParm, ccColor4F color)
{
    if(innerRadius > radius || timeCircle <= 0.0f || alphaParm < 0.0f)
        return false;
    if(timeCircle <= 0.1f)
        timeCircle += 0.2f;
    m_innerRadius = innerRadius;
    m_radius = radius;
    m_timeCircle = timeCircle;
    m_alphaParm = alphaParm;
    m_color = color;

    m_watchLayer = NULL;

    loadShaderVertex("circlelight.vsh", "circlelight.fsh");

    setContentSize(CCSizeMake(m_radius * 2, m_radius * 2));

    m_blendFunc.src = GL_SRC_ALPHA;
    m_blendFunc.dst = GL_ZERO;

    m_time = 0.0f;
    this->scheduleUpdate();
    return true;
}

bool CircleLight::initWithRadius(int innerRadius, int radius)
{
    return initWithAllParm(innerRadius, radius, 3.0f, 0.0f, ccc4f(1.0f, 0.0f, 0.0f, 0.0f));

    //loadShaderVertex("circlelight.vsh", "circlelight.fsh");
    //m_innerRadius = innerRadius;
    //m_radius = radius;
    //m_utexture = CCTextureCache::sharedTextureCache()->addImage("CloseNormal.png")->getName();

    //setContentSize(CCSizeMake(100, 100));
    //setColor(ccc4f(0.5, 0.5, 0.5, 1));

    ////m_blendFunc = ccBlendFunc(GL_SRC_ALPHA, GL_ONE);
    //m_blendFunc.src = GL_SRC_ALPHA;
    //m_blendFunc.dst = GL_ONE;

    //m_time = 0.0f;
    //this->scheduleUpdate();
    //return true;
}

void CircleLight::loadShaderVertex(const char *vert, const char *frag)
{
    CCGLProgram *shader = new CCGLProgram();
    
    shader->initWithVertexShaderFilename(vert, frag);
   
    //CCLog("Shader log : %s", shader->fragmentShaderLog());
    
    shader->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
    shader->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
    shader->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);

    shader->link();
    //shader->vertexShaderLog();

    //CCLog("%s", glGetString(GL_SHADING_LANGUAGE_VERSION));
    shader->updateUniforms();
    m_ucenter = glGetUniformLocation(shader->getProgram(), "center");
    m_uinnerRadius = glGetUniformLocation(shader->getProgram(), "u_innerRadius");
    m_uradius = glGetUniformLocation(shader->getProgram(), "u_radius");
    m_utime = glGetUniformLocation(shader->getProgram(), "u_time");
    m_utimeCircle = glGetUniformLocation(shader->getProgram(), "u_timeCircle");
    m_ualphaParm = glGetUniformLocation(shader->getProgram(), "u_alphaParm");
    m_ucolor = glGetUniformLocation(shader->getProgram(), "u_color");

    this->setShaderProgram(shader);
    shader->release();
}

void CircleLight::setColor(ccColor4F newColor)
{
    m_color = newColor;
}

void CircleLight::setWatchLayer(CCLayer *layer, int distance /* = 0 */)
{
    m_watchLayer = layer;
    
    m_distance = distance;

    m_initialHeight = m_watchLayer->getPositionY();

    this->setZOrder(-m_distance);
}

void CircleLight::setRelativePosition(CCPoint point)
{
    m_relativePosition = point;
   
}

void CircleLight::update(float delta)
{
    m_time += delta;

    if(m_watchLayer != NULL)
    {
        int yOffset = (m_watchLayer->getPositionY() - m_initialHeight) / m_distance;
        
        this->setPosition(ccp(m_relativePosition.x, m_relativePosition.y + yOffset));
    }

    if(this->getPositionY() < -100)
    {
        this->getParent()->removeChild(this);
    }

    //CCLog("light position : (%f, %f)", this->getPositionX(), this->getPositionY());
}

void CircleLight::setContentSize(const CCSize& contentSize)
{
    CCNode::setContentSize(contentSize);

    m_center.x = contentSize.width / 2.0f;
    m_center.y = contentSize.height/2.0f;
}

void CircleLight::draw()
{
    CC_NODE_DRAW_SETUP();
    ccGLBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    CCGLProgram *shader = getShaderProgram();
    shader->setUniformLocationWith2f(m_ucenter, m_center.x, m_center.y);
    shader->setUniformLocationWith1f(m_uinnerRadius, m_innerRadius);
    shader->setUniformLocationWith1f(m_uradius, m_radius);
    shader->setUniformLocationWith1f(m_utime, m_time);
    shader->setUniformLocationWith1f(m_utimeCircle, m_timeCircle);
    shader->setUniformLocationWith1f(m_ualphaParm, m_alphaParm);
    shader->setUniformLocationWith4f(m_ucolor, m_color.r, m_color.g, m_color.b, m_color.a);
    
    CCSize size = this->getContentSize();
    float w = size.width;
    float h = size.height;

  
    //ccGLBindTexture2D(GL_TEXTURE_2D);
    //glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, w, h, 0);
    glEnableVertexAttribArray(kCCVertexAttrib_Position);
    //ccGLBlendFunc(m_blendFunc.src, m_blendFunc.dst);
    //glDisableVertexAttribArray(kCCVertexAttrib_Color);

    GLfloat vertices[8] = {
        0, 0,
        w, 0,
        w, h,
        0, h,
    };

    GLfloat vertices1[12] = {
        0, 0,
        w, 0,
        w, h,
        0, 0,
        w, h,
        0, h,
    };


    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices1);
    //glVertexAttrib4fv(kCCVertexAttrib_Color, color);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}