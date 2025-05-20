#ifndef _VARIABLE__H
#define _VARIABLE__H

#include "graphics.h"
#include "defs.h"
#include <bits/stdc++.h>
#include "SDL2/SDL.h"
#include "SDL_ttf.h"
#include "SDL2/SDL_image.h"
#include "entities.h"
#include "shockwave.h"
#include "luckybox.h"
#include "SDL_mixer.h"

using namespace std;

extern vector<ShockWave> shockWaves;

const int xSpawn[] = {0, RANGE_RANDOM_ENEMY, -1};
const int ySpawn[] = {-1, -1, 0};
const Uint32 SHOCK_COOLDOWN = 1500;
const Uint32 BULLET_COOLDOWN = 1000;
const Uint32 LUCKYBOX_COOLDOWN = 5000;

SDL_Surface* cursorPointer = IMG_Load("Pop Cat Meme Animated - pointer - SweezyCursors.png");
SDL_Surface* cursorHover = IMG_Load("Pop Cat Meme Animated--cursor--SweezyCursors.png");
SDL_Cursor* customCursorPointer = SDL_CreateColorCursor(cursorPointer, 10, 10);
SDL_Cursor* customCursorHover = SDL_CreateColorCursor(cursorHover, 0, 0);

const string typeOfEffect[] = {"s :x2 Score", "s :x2 Money", "s :ShockWave's CD"};

vector<ShockWave> shockWaves;
MenuBackground menuBackground;
Graphics graphics;
Crosshair crosshair;
ScrollingBackground background;
ArmyTank tank;
vector <Zombie> zombies;
bool quit = false;
bool running = true;
SDL_Event e;
int kills = 0;
vector <Bullet> bullets;
int cntBullets;
int coins;
vector <LuckyBox> luckyBoxs;
bool x2Score = false, x2Money = false;
vector <int> cooldownEffects;
set <int> checkActiveEffect;

// Time

Uint32 x2ScoreStart = 0, x2MoneyStart = 0;
Uint32 lastLuckyBoxTime = 0;
Uint32 lastShockTime = 0;
Uint32 lastReloadBullet = 0;

// Music

Mix_Music* backgroundMusic = nullptr;
Mix_Chunk* effectShoot = nullptr;
Mix_Chunk* effectZombieDie = nullptr;
Mix_Chunk* effectBuy = nullptr;
Mix_Chunk* effectHover = nullptr;
Mix_Chunk* effectGameOver = nullptr;

#endif // _VARIABLE__H
