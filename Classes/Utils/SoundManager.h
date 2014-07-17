#ifndef __SOUND_MANAGER__
#define __SOUND_MANAGER__

#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

enum Effect
{
    EF_JUMP,
    EF_HOOK,
    EF_PILL,
    EF_END,
    EF_WAVE, 
    EF_SUPERJUMP
};

class SoundManager
{
public:
    ~SoundManager();

    static SoundManager* Instance();

    void makeEffect(Effect ef);

    SimpleAudioEngine *getEngine()
    {
        return SimpleAudioEngine::sharedEngine();
    }
protected:
    SoundManager();
private:
    
};
#endif