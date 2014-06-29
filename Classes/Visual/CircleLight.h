#include "cocos2d.h"

USING_NS_CC;

class CircleLight : public CCNode
{
public:
    CircleLight();

    bool initWithRadius(int innerRadius, int radius);
    bool initWithAllParm(int innerRadius, int radius, float timeCircle, float alphaParm, ccColor4F color);
    void loadShaderVertex(const char *vert, const char *frag);

    virtual void update(float delta);
    virtual void setContentSize(const CCSize& contentSize);
    virtual void setColor(ccColor4F newColor);
    virtual void draw();
    void setRelativePosition(CCPoint point);

    static CircleLight* createCircleLight(int innerRadius, int radius);
    static CircleLight* createCircleLight(int innerRadius, int radius, float timeCircle, float alphaParm, ccColor4F color);

    void setWatchLayer(CCLayer *layer, int distance = 0);
private:
    GLuint m_ucenter, m_uinnerRadius, m_uradius, m_utime, m_ualphaParm, m_utimeCircle, m_ucolor;
    GLuint m_attributeColor, m_attributePosition;
    GLuint m_utexture;

    GLuint m_uniformTex0;
    GLfloat m_innerRadius, m_radius, m_time, m_alphaParm, m_timeCircle;
    ccVertex2F m_center;
    ccColor4F m_color;
    GLfloat color[4];

    ccBlendFunc m_blendFunc;// = {GL_SRC_ALPHA, GL_ONE};  

    int m_distance;
    CCLayer *m_watchLayer;

    CCPoint m_relativePosition;
};