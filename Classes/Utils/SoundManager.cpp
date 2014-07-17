#include "SoundManager.h"

SoundManager::SoundManager()
{
    SimpleAudioEngine::sharedEngine()->preloadEffect("wave.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("hook.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("pill.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("end.mp3");
    SimpleAudioEngine::sharedEngine()->preloadEffect("jump.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("superjump.wav");


}

SoundManager::~SoundManager()
{

}

SoundManager* SoundManager::Instance()
{
    static SoundManager* instance = new SoundManager();

    return instance;
}

void SoundManager::makeEffect(Effect ef)
{
    switch (ef)
    {
    case EF_JUMP:
        SimpleAudioEngine::sharedEngine()->playEffect("jump.wav");
        break;
    case EF_HOOK:
        SimpleAudioEngine::sharedEngine()->playEffect("hook.wav");
        break;
    case EF_PILL:
        SimpleAudioEngine::sharedEngine()->playEffect("pill.wav");
        break;
    case EF_END:
        SimpleAudioEngine::sharedEngine()->playEffect("end.mp3");
        break;
    case EF_WAVE:
        SimpleAudioEngine::sharedEngine()->playEffect("wave.wav");
        break;
    case EF_SUPERJUMP:
        SimpleAudioEngine::sharedEngine()->playEffect("superjump.wav");
        break;
    default:
        break;
    }
}