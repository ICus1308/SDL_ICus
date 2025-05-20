#ifndef _MUSIC__H
#define _MUSIC__H

#include <bits/stdc++.h>
#include "SDL_mixer.h"
#include "graphics.h"
#include "variable.h"

Mix_Chunk* loadEffect(const char* path) {
    Mix_Chunk* chunk = Mix_LoadWAV(path);
    if (chunk == nullptr) {
        SDL_Log("Could not load sound effect %s! SDL_mixer Error: %s", path, Mix_GetError());
    }
    return chunk;
}

void loadAllSound() {
    backgroundMusic = graphics.loadMusic("background.mp3");
    effectShoot = loadEffect("shoot.wav");
    effectZombieDie = loadEffect("die.wav");
    effectBuy = loadEffect("buy.wav");
    effectHover = loadEffect("start_hover.wav");
    effectGameOver = loadEffect("gameover.wav");
}

void freeAllSounds() {
    if (backgroundMusic) {
        Mix_FreeMusic(backgroundMusic);
        backgroundMusic = nullptr;
    }
    if (effectShoot) {
        Mix_FreeChunk(effectShoot);
        effectShoot = nullptr;
    }
    if (effectZombieDie) {
        Mix_FreeChunk(effectZombieDie);
        effectZombieDie = nullptr;
    }
    if (effectBuy) {
        Mix_FreeChunk(effectBuy);
        effectBuy = nullptr;
    }
    if (effectHover) {
        Mix_FreeChunk(effectHover);
        effectHover = nullptr;
    }
    if (effectGameOver) {
        Mix_FreeChunk(effectGameOver);
        effectGameOver = nullptr;
    }
}

#endif // _MUSIC__H
