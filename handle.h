#ifndef _HANDLE__H
#define _HANDLE__H

#include "SDL2/SDL.h"
#include <SDL2/SDL_image.h>
#include "defs.h"
#include "graphics.h"
#include "entities.h"
#include <bits/stdc++.h>
#include "variable.h"

using namespace std;

void applyLuckyBoxEffect(int option, Uint32 currentTime) {
    switch(option) {
        case LB_X2_SCORE:
            x2Score = true;
            x2ScoreStart = currentTime;
            break;
        case LB_X2_MONEY:
            x2Money = true;
            x2MoneyStart = currentTime;
            break;
        case LB_SPAWN_ZOMBIES: {
            int x0 = 50 + rand() % (SCREEN_WIDTH + 1);
            int y0 = 50;
            for (int i = 0; i < 5; ++i) {
                int dx = rand() % 50 - 25, dy = rand() % 50 - 25;
                Zombie zb(x0 + dx, y0 + dy, graphics.loadTexture(ZOMBIE_IMG));
                zb.getReach(tank);
                zombies.push_back(zb);
            }
            break;
        }
    }
}

bool handleTankCollision (ArmyTank tank, Zombie zombie) {
    vector <vector <int>> visited(800, vector <int> (800, 0));
    for (int i = tank.xBody; i <= tank.xBody + tank.wBody; i++)
        for (int j = tank.yBody; j <= tank.yBody + tank.hBody; j++)
            visited[i][j]++;

    for (int i = zombie.xZombie; i <= zombie.xZombie + zombie.width; i++)
        for (int j = zombie.yZombie; j <= zombie.yZombie + zombie.height; j++)
            visited[i][j]++;

    for (int i = 0; i <= 700; i++)
        for (int j = 0; j <= 700; j++)
            if (visited[i][j] > 1)
                return false;

    return true;
}

void handleBulletCollision(int checkPiercing) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    float barrelLength = 3 * tank.hTurrent / 4;
    float cx = SCREEN_WIDTH_CENTER;
    float cy = tank.yTurrent + barrelLength;

    if ((mouseX - cx) * (mouseX - cx) +
        (mouseY - cy) * (mouseY - cy) <= barrelLength * barrelLength)
            return;

    vector <Zombie> curZombiesLiving;
    vector <Bullet> curBullets;
    for (int i = 0; i < bullets.size(); i++) {
        bool checkCollision = false;

        for (int j = 0; j < zombies.size(); j++) {
            if (zombies[j].xZombie <= bullets[i].x &&
                bullets[i].x <= zombies[j].xZombie + zombies[j].width &&
                zombies[j].yZombie <= bullets[i].y &&
                bullets[i].y <= zombies[j].yZombie + zombies[j].height) {
                    SDL_DestroyTexture(zombies[j].texture);
                    if (x2Score) kills += 2; else kills++;
                    if (x2Money) coins += 2; else coins++;
                    checkCollision = true;
                    continue;
                }

            curZombiesLiving.push_back(zombies[j]);
        }

        if (checkCollision) {
            zombies.clear();
            for (int i = 0; i < curZombiesLiving.size(); i++)
                zombies.push_back(curZombiesLiving[i]);

            if (checkPiercing)
                curBullets.push_back(bullets[i]);
        }
        else
            curBullets.push_back(bullets[i]);
        curZombiesLiving.clear();
    }

    bullets.clear();
    for (int i = 0; i < curBullets.size(); i++) {
        if (0 <= curBullets[i].x && curBullets[i].x <= SCREEN_WIDTH &&
            0 <= curBullets[i].y && curBullets[i].y <= SCREEN_HEIGHT)
                bullets.push_back(curBullets[i]);
    }

    for (Bullet& bullet : bullets) {
        for (auto& lb : luckyBoxs) {
            if (lb.alive && lb.x <= bullet.x &&
                bullet.x <= lb.x + 48 &&
                lb.y <= bullet.y &&
                bullet.y <= lb.y + 48) {
                    lb.alive = false;
                    int option = lb.effect;
                    if (option < 2 && checkActiveEffect.find(option) == checkActiveEffect.end()) {
                        cooldownEffects.push_back(option);
                        checkActiveEffect.insert(option);
                    }
                    applyLuckyBoxEffect(option, SDL_GetTicks());
            }
        }
    }
}

void handleWaveCollision() {
    vector <Zombie> curZombieLiving;
    for (auto &sw : shockWaves) {
        for (size_t i = 0; i < zombies.size(); i++) {
            if (sw.hits(zombies[i])) {
                SDL_DestroyTexture(zombies[i].texture);
                if (x2Score) kills += 2; else kills++;
                if (x2Money) coins += 2; else coins++;
            } else
                curZombieLiving.push_back(zombies[i]);
        }

        zombies.clear();
        for (int i = 0; i < curZombieLiving.size(); i++)
            zombies.push_back(curZombieLiving[i]);
    }

    shockWaves.erase(
        remove_if(shockWaves.begin(), shockWaves.end(),
            [](const ShockWave &sw){ return !sw.active; }
        ),
        shockWaves.end()
    );
}

void handleWallLimitCollision() {
    vector <Zombie> curZombieLiving;
    for (int i = 0; i < zombies.size(); i++) {
        if (zombies[i].yZombie >= SCREEN_HEIGHT)
            SDL_DestroyTexture(zombies[i].texture);
        else
            curZombieLiving.push_back(zombies[i]);
    }

    zombies.clear();
    for (int i = 0; i < curZombieLiving.size(); i++)
        zombies.push_back(curZombieLiving[i]);
}

#endif // _HANDLE__H
