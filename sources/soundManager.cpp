#include "soundmanager.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>

EM_JS(void, playWebSound, (const char* path, bool preventOverlap), {

    if (!Module.audioCache) {
        Module.audioCache = {};
    }

    const filename = UTF8ToString(path);

    if (!Module.audioCache[filename]) {
        Module.audioCache[filename] = new Audio(filename);
    }

    const audio = Module.audioCache[filename];

    if (!preventOverlap || audio.paused || audio.ended) {
        audio.currentTime = 0;
        audio.play();
    }
});

#endif


SoundManager::SoundManager()
{
#ifndef __EMSCRIPTEN__

    eatenGhostSound = new QSoundEffect();
    extraLiveSound = new QSoundEffect();
    eatenDotSound = new QSoundEffect();
    beginningSound = new QSoundEffect();
    deathSound = new QSoundEffect();
    eatenFruitSound = new QSoundEffect();


    eatenGhostSound->setSource(
        QUrl("qrc:/Images/sounds/pacman-eatghost/pacman_eatghost.wav")
        );

    extraLiveSound->setSource(
        QUrl("qrc:/Images/sounds/pacman-extrapac/pacman_extrapac.wav")
        );

    eatenDotSound->setSource(
        QUrl("qrc:/Images/sounds/pacman-chomp/short2_pacman_chomp.wav")
        );

    beginningSound->setSource(
        QUrl("qrc:/Images/sounds/pacman-beginning/pacman_beginning.wav")
        );

    deathSound->setSource(
        QUrl("qrc:/Images/sounds/pacman-death/pacman_death.wav")
        );

    eatenFruitSound->setSource(
        QUrl("qrc:/Images/sounds/pacman-eatfruit/pacman_eatfruit.wav")
        );

#endif
}


void SoundManager::playEatenDotSound()
{
#ifdef __EMSCRIPTEN__

    playWebSound("sounds/pacman_chomp.wav", true);

#else

    if (!eatenDotSound->isPlaying())
        eatenDotSound->play();

#endif
}


void SoundManager::playDeathSound()
{
#ifdef __EMSCRIPTEN__

    playWebSound("sounds/pacman_death.wav", false);

#else

    deathSound->play();

#endif
}


void SoundManager::playBeginningSound()
{
#ifdef __EMSCRIPTEN__

    playWebSound("sounds/pacman_beginning.wav", false);

#else

    beginningSound->play();

#endif
}


void SoundManager::playEatenGhostSound()
{
#ifdef __EMSCRIPTEN__

    playWebSound("sounds/pacman_eatghost.wav", false);

#else

    eatenGhostSound->play();

#endif
}


void SoundManager::playExtraLiveSound()
{
#ifdef __EMSCRIPTEN__

    playWebSound("sounds/pacman_extrapac.wav", false);

#else

    extraLiveSound->play();

#endif
}


void SoundManager::playEatenFruitSound()
{
#ifdef __EMSCRIPTEN__

    playWebSound("sounds/pacman_eatfruit.wav", false);

#else

    eatenFruitSound->play();

#endif
}