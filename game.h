#ifndef _GAME__H
#define _GAME__H

#include "SDL2/SDL.h"
#include <bits/stdc++.h>
#include "SDL2/SDL_image.h"
#include "entities.h"
#include "defs.h"
#include "variable.h"
#include "coin.h"
#include "shockwave.h"
#include "shop.h"
#include "highscore.h"
#include "graphics.h"

using namespace std;

int ran(int l, int r) {
    return rand() % (r - l + 1) + l;
}

string numberToText(int number) {
    string ans;
    while (number) {
        ans.push_back(number % 10 + '0');
        number /= 10;
    }

    reverse (ans.begin(), ans.end());

    if (ans.empty())
        return "0";
    return ans;
}

void randomZombie() {
    if (!ran(0, 30) && zombies.size() < 5) {
        int direction = ran(0, 2);

        int x = xSpawn[direction];
        int y = ySpawn[direction];

        if (x == -1)
            x = ran(0, RANGE_RANDOM_ENEMY);
        if (y == -1)
            y = ran(0, SCREEN_WIDTH / 2);

        Zombie zombie(x, y, graphics.loadTexture(ZOMBIE_IMG));
        zombie.getReach(tank);

        zombies.push_back(zombie);
    }

    for (int i = 0; i < zombies.size(); i++) {
        zombies[i].render(graphics.renderer);
        zombies[i].update(tank, kills);
        if (!handleTankCollision(tank, zombies[i])) {
            quit = true;
            break;
        }
    }
}

void effectCooldown() {
    Uint32 now = SDL_GetTicks();
    if (x2Score && now - x2ScoreStart > 10000)
        x2Score = false;
    if (x2Money && now - x2MoneyStart > 10000)
        x2Money = false;
}

void spawnLuckyBox() {
    Uint32 timeNow = SDL_GetTicks();
    if (timeNow - lastLuckyBoxTime > LUCKYBOX_COOLDOWN && !ran(0, 2)) {
        lastLuckyBoxTime = timeNow;

        int effectType = rand() % 3;
        LuckyBoxEffect effect = (LuckyBoxEffect)effectType;
        LuckyBox lb(-48, 50 + rand() % 50, 3.5f, graphics.loadTexture(LUCKYBOX_IMG), effect);
        luckyBoxs.push_back(lb);
    }
    if (timeNow - lastLuckyBoxTime > LUCKYBOX_COOLDOWN)
        lastLuckyBoxTime = timeNow;
}

void renderInfomation() {
    graphics.loadFont("Playthings.ttf", 24);
    SDL_Color color = {255, 255, 255, 0};
    string curText = "Kills " + numberToText(kills);
    SDL_Texture *text = graphics.renderText(curText.c_str(), color);
    graphics.renderTexture(text, 10, 10, 1);

    SDL_Texture *curCoins = graphics.renderText(("Coins: " + numberToText(coins)).c_str(), color);
    graphics.renderTexture(curCoins, 10, 35, 1);

    SDL_Texture *curBullet = graphics.renderText(("Bullets: " + numberToText(cntBullets)).c_str(), color);
    graphics.renderTexture(curBullet, 10, 60, 1);

    vector <int> curActiveEffects;
    for (int i = 0; i < cooldownEffects.size(); i++) {
        int timeNow = 10000 - (SDL_GetTicks() - (cooldownEffects[i] == 0 ? x2ScoreStart : x2MoneyStart));
        if (cooldownEffects[i] == 2)
            timeNow = 1500 - (SDL_GetTicks() - lastShockTime);

        float seconds = timeNow / 1000.0;
        if (seconds <= 0)
            continue;

        curActiveEffects.push_back(cooldownEffects[i]);

        ostringstream ss;
        ss << fixed << setprecision(2) << seconds;
        string timeText = ss.str();

        SDL_Texture *curEffect = graphics.renderText((timeText + typeOfEffect[cooldownEffects[i]]).c_str(), color);
        int w;
        SDL_QueryTexture(curEffect, NULL, NULL, &w, NULL);
        graphics.renderTexture(curEffect, SCREEN_WIDTH - w - 10, 650 - i * 25, 1);

        SDL_DestroyTexture(curEffect);
    }

    cooldownEffects.clear();
    checkActiveEffect.clear();
    for (int x : curActiveEffects) {
        cooldownEffects.push_back(x);
        checkActiveEffect.insert(x);
    }

//    graphics.loadFont("DroplineRegular-Wpegz.otf", 24);
//    SDL_Texture* wayOut = graphics.renderText("Press ESC to exit", {255, 215, 0, 255});
//    graphics.renderTexture(wayOut, 10, 650, 1);
//    SDL_DestroyTexture(wayOut);

    SDL_DestroyTexture(text);
    SDL_DestroyTexture(curCoins);
    SDL_DestroyTexture(curBullet);
}

void updateLuckyBox() {
    spawnLuckyBox();
    for (auto& lb : luckyBoxs)
        if (lb.alive) lb.update();

    for (auto& lb : luckyBoxs)
        if (lb.alive) lb.render(graphics);

    for (LuckyBox& lb : luckyBoxs)
        if (!lb.alive && lb.texture) {
            SDL_DestroyTexture(lb.texture);
            lb.texture = nullptr;
        }

    luckyBoxs.erase(
        remove_if(luckyBoxs.begin(), luckyBoxs.end(),
                       [](const LuckyBox& lb) { return !lb.alive; }),
        luckyBoxs.end()
    );
}

void reloadBullet() {
    Uint32 timeNow = SDL_GetTicks();
    if (cntBullets < 5 && timeNow - lastReloadBullet >= BULLET_COOLDOWN) {
        cntBullets++;
        lastReloadBullet = timeNow;
    }

    if (cntBullets >= 5)
        lastReloadBullet = timeNow;
}

void gameInit() {
    x2Score = false;
    x2Money = false;
    x2ScoreStart = 0;
    x2MoneyStart = 0;
    luckyBoxs.clear();

    cntBullets = 5;
    bullets.clear();
    quit = false;
    zombies.clear();
    kills = 0;
    tank.reset();
    SDL_ShowCursor(SDL_DISABLE);
    crosshair.setTexture(graphics.loadTexture(CROSSHAIR_IMG));
    tank.setTextureBody(graphics.loadTexture(TANK_IMG));
    tank.setTextureTurrent(graphics.loadTexture(GUN_IMG));
}

void gamePlay() {
    while( !quit ) {
        graphics.renderBackground(background);
        randomZombie();
        graphics.renderTexture(tank.body, tank.xBody, tank.yBody, 2);
        graphics.renderTextureEx(tank.turrent, tank.xTurrent, tank.yTurrent, 2);
        crosshair.update();
        crosshair.render(graphics.renderer);

        for (auto &sw : shockWaves)
            sw.update();
        handleWaveCollision();
        for (auto &sw : shockWaves)
            sw.render(graphics);

        for (Bullet &x : bullets)
            x.update();
        handleBulletCollision(shopItems[0].bought);
        for (Bullet &x : bullets)
            x.render(graphics);

        renderInfomation();

        handleWallLimitCollision();

        updateLuckyBox();

        effectCooldown();

        reloadBullet();

        while( SDL_PollEvent( &e ) != 0 ) {
            if( e.type == SDL_QUIT ) {
                running = false;
                quit = true;
            }

            else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) { // Is clicked ?
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                float barrelLength = 3 * tank.hTurrent / 4;
                float cx = SCREEN_WIDTH_CENTER;
                float cy = tank.yTurrent + barrelLength;

                if ((mouseX - cx) * (mouseX - cx) +
                    (mouseY - cy) * (mouseY - cy) <= barrelLength * barrelLength)
                        continue;

                Bullet curBullet = tank.drawBullet(graphics.renderer);
                if (curBullet.x != 0 && cntBullets > 0) {
                    cntBullets--;
                    bullets.push_back(curBullet);
                    graphics.play(effectShoot);
                }
            }

            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q && shopItems[1].bought == true) { // Press q ?
                Uint32 timeNow = SDL_GetTicks();
                if (timeNow - lastShockTime >= SHOCK_COOLDOWN) {
                    lastShockTime = timeNow;
                    int cx = SCREEN_WIDTH_CENTER;
                    int cy = tank.yTurrent + tank.hTurrent * 3 / 4;
                    int radius = 3 * tank.hTurrent / 4 + 50;
                    shockWaves.emplace_back(cx, cy, radius, 4, 20);

                    if (checkActiveEffect.find(2) == checkActiveEffect.end())
                        cooldownEffects.push_back(2);
                }
            }

            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
                quit = true;
        }

        background.scroll(1);

        graphics.presentScene();
        SDL_Delay(20);
    }
}

void gameOut() {
    SDL_ShowCursor(1);

    SDL_Color color = {255, 255, 255, 0};
    graphics.prepareScene(menuBackground.texture);
    graphics.loadFont("Playthings.ttf", 72);
    SDL_Texture* titleTex = graphics.renderText("GAME OVER", color);
    SDL_Rect titleRect; SDL_QueryTexture(titleTex, nullptr, nullptr, &titleRect.w, &titleRect.h);
    titleRect.x = (SCREEN_WIDTH - titleRect.w)/2;
    titleRect.y = 100;
    SDL_RenderCopy(graphics.renderer, titleTex, nullptr, &titleRect);
    SDL_DestroyTexture(titleTex);

    graphics.loadFont("Playthings.ttf", 36);
    string finalScore = "Your score: " + to_string(kills);
    SDL_Texture* score = graphics.renderText(finalScore.c_str(), color);
    SDL_Rect scoreRect; SDL_QueryTexture(score, nullptr, nullptr, &scoreRect.w, &scoreRect.h);
    scoreRect.x = (SCREEN_WIDTH - scoreRect.w)/2;
    scoreRect.y = 230;
    SDL_RenderCopy(graphics.renderer, score, nullptr, &scoreRect);
    SDL_DestroyTexture(score);

    graphics.presentScene();

    graphics.play(effectGameOver);
    SDL_Delay(3380);

    saveNewDataOfCoin();
    saveNewScore();
    SDL_DestroyTexture( tank.body );
    SDL_DestroyTexture( tank.turrent );
    for (Zombie x : zombies)
        SDL_DestroyTexture(x.texture);
    for (LuckyBox x : luckyBoxs)
        SDL_DestroyTexture(x.texture);
    cooldownEffects.clear();
}

#endif // _GAME__H
