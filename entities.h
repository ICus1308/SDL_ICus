#ifndef _ENTITIES__H
#define _ENTITIES__H

#include "SDL2/SDL.h"
#include <SDL2/SDL_image.h>
#include "defs.h"
#include "graphics.h"
#include <bits/stdc++.h>

using namespace std;

struct Bullet {
    float x;
    float y;
    float vx;
    float vy;

    void update() {
        x += vx;
        y += vy;
    }

    void render(Graphics graphics) {
        for (int i = 1; i <= 5; i++)
            graphics.drawCircle(x, y, i);
    }
};

struct ArmyTank {
    SDL_Texture* body;
    SDL_Texture* turrent;
    int xBody = SCREEN_WIDTH_CENTER;
    int yBody = SCREEN_HEIGHT_CENTER;
    int wBody, hBody;

    int xTurrent = SCREEN_WIDTH_CENTER;
    int yTurrent = SCREEN_HEIGHT_CENTER;
    int wTurrent, hTurrent;

    void reset() {
        xTurrent = SCREEN_WIDTH_CENTER;
        yTurrent = SCREEN_HEIGHT_CENTER;

        xBody = SCREEN_WIDTH_CENTER;
        yBody = SCREEN_HEIGHT_CENTER;
    }

    void setTextureBody(SDL_Texture* _texture) {
        body = _texture;
        SDL_QueryTexture(body, NULL, NULL, &wBody, &hBody);

        wBody /= 2;
        hBody /= 2;

        xBody -= wBody / 2;
        yBody -= hBody / 2 - 35 - 100;
    }

    void setTextureTurrent(SDL_Texture* _texture) {
        turrent = _texture;
        SDL_QueryTexture(turrent, NULL, NULL, &wTurrent, &hTurrent);

        wTurrent /= 2;
        hTurrent /= 2;

        xTurrent -= wTurrent / 2;
        yTurrent -= hTurrent / 2 + 35 - 100;
    }

    Bullet drawBullet(SDL_Renderer* renderer) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

        int cx = SCREEN_WIDTH_CENTER;
        int cy = yTurrent + 3 * hTurrent / 4;

        float dx = mouseX - cx;
        float dy = mouseY - cy;
        float angleRad = atan2(dy, dx);

        float barrelLength = 3 * hTurrent / 4;
        float headX = cx + cos(angleRad) * barrelLength;
        float headY = cy + sin(angleRad) * barrelLength;

//        float normalX = -sin(angleRad);
//        float normalY = cos(angleRad);

        if (sqrt((mouseX - cx) * (mouseX - cx) + (mouseY - cy) * (mouseY - cy)) >= barrelLength) {
            float dx = mouseX - headX;
            float dy = mouseY - headY;
            float len = sqrt(dx * dx + dy * dy);

            float vx = dx / len * 8;
            float vy = dy / len * 8;

            return{headX, headY, vx, vy};

//            for (int i = -2; i <= 2; i++)
//                SDL_RenderDrawLine(renderer, headX + i * normalX, headY + i * normalY, mouseX + i * normalX, mouseY + i * normalY);
        }
    }
};

struct Edge {
    int x1, y1, x2, y2;
};

int tx[] = {0, 1, 2, 0, 1, 2, 0, 1};
int ty[] = {0, 0, 0, 1, 1, 1, 2, 2};

struct Zombie {
    SDL_Texture *texture;
    float xZombie, yZombie;
    float xReach, yReach;
    float vx, vy;
    const int width = 48, height = 48;

    Zombie(int _x, int _y, SDL_Texture *_texture) {
        xZombie = _x;
        yZombie = _y;
        texture = _texture;
    }

    void getReach(ArmyTank tank) {
        std::vector <Edge> edges;

        edges.push_back({
            tank.xBody,
            tank.yBody,
            tank.xBody + tank.wBody,
            tank.yBody
        });
        edges.push_back({
            tank.xBody,
            tank.yBody,
            tank.xBody,
            tank.yBody + tank.hBody
        });
        edges.push_back({
            tank.xBody,
            tank.yBody + tank.hBody,
            tank.xBody + tank.wBody,
            tank.yBody + tank.hBody
        });
        edges.push_back({
            tank.xBody + tank.wBody,
            tank.yBody,
            tank.xBody + tank.wBody,
            tank.yBody + tank.hBody
        });

        float distance = 1400;
        int xCur, yCur;

        for (int i = 0; i < 4; i++) {
            Edge e = edges[i];

            if (i % 2) {
                xCur = e.x1;
                if (yZombie >= tank.yBody + tank.hBody / 2)
                    yCur = min<float> (e.y2, yZombie);
                else
                    yCur = max<float> (e.y1, yZombie);
            }
            else {
                yCur = e.y1;
                if (xZombie >= tank.xBody + tank.wBody / 2)
                    xCur = min<float> (e.x2, xZombie);
                else
                    xCur = max<float> (e.x1, xZombie);
            }

            float curDistance = sqrt((xZombie - xCur) * (xZombie - xCur) + (yZombie - yCur) * (yZombie - yCur));
            if (curDistance < distance) {
                distance = curDistance;
                xReach = xCur;
                yReach = yCur;
            }
        }
    }

    int Time = 0;

    void update(ArmyTank& tank, int cntKill) {
        getReach(tank);

        float dx = xReach - (xZombie + 24);
        float dy = yReach - (yZombie + 24);
        float len = sqrt(dx * dx + dy * dy);

        int speed = cntKill / 7 + 1;
        vx = dx / len * speed * 0.5;
        vy = dy / len * speed * 0.5;

        xZombie += vx;
        yZombie += vy + 1;

        Time++;
        Time %= 8;
    }

    void render(SDL_Renderer *renderer) {
        SDL_Rect srcdest;
        srcdest.x = width * tx[Time];
        srcdest.y = height * ty[Time];
        srcdest.w = width;
        srcdest.h = height;

        SDL_Rect dstdest;
        dstdest.x = xZombie;
        dstdest.y = yZombie;
        dstdest.w = width;
        dstdest.h = height;

        float dx = xReach - xZombie;
        float dy = yReach - yZombie;
        float angle = atan2(dy, dx) * 180.0 / 3.14;

        SDL_Point point = {24, 24};

        SDL_RenderCopyEx(renderer, texture, &srcdest, &dstdest, angle, &point, SDL_FLIP_NONE);
    }
};

#endif // _ENTITIES__H
