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

using namespace std;

extern vector<ShockWave> shockWaves;

const int xSpawn[] = {0, RANGE_RANDOM_ENEMY, -1};
const int ySpawn[] = {-1, -1, 0};
const Uint32 SHOCK_COOLDOWN = 1500;
const Uint32 BULLET_COOLDOWN = 1000;
const Uint32 LUCKYBOX_COOLDOWN = 5000;
string typeOfEffect[] = {"s :x2 Score", "s :x2 Money", "s :ShockWave's CD"};

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

#endif // _VARIABLE__H
