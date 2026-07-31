#pragma once

#ifndef __EMSCRIPTEN__
#include <QSoundEffect>
#endif

#include <QUrl>

class SoundManager
{
public:
    SoundManager();

    void playEatenDotSound();
    void playDeathSound();
    void playBeginningSound();
    void playEatenGhostSound();
    void playExtraLiveSound();
    void playEatenFruitSound();

private:

#ifndef __EMSCRIPTEN__
    QSoundEffect* eatenGhostSound;
    QSoundEffect* extraLiveSound;
    QSoundEffect* eatenDotSound;
    QSoundEffect* beginningSound;
    QSoundEffect* deathSound;
    QSoundEffect* eatenFruitSound;
#endif
};