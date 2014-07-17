#ifndef __SCORE_BOARD__
#define __SCORE_BOARD__


#include "cocos2d.h"

USING_NS_CC;

class ScoreBoard : public cocos2d::CCNode
{
public:
    ~ScoreBoard();

    static ScoreBoard* createScoreBoard(CCNode* layer);

    bool initWithRole(CCNode *role);

    void setScore(int height);

    void scoreUpdate(float dt);

    void reset();

    void addBonus(int bouns = 0);
    /**this will clear the m_lastBonus score, make the addtional bonus start from 10*/
    void clearBonus();
private:
    ScoreBoard();


public:
        int m_sumScore;
        int m_scoreRate;
private:
    CCNode* m_layer;
    CCLabelAtlas *m_label;


    int m_bonusSumScore;

    int m_lastBonus;
    
};

#endif