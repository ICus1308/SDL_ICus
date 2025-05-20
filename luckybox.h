#ifndef _LUCKYBOX__H
#define _LUCKYBOX__H

#include <cmath>
#include "SDL2/SDL.h"
#include "defs.h"

enum LuckyBoxEffect {
    LB_X2_SCORE = 0,
    LB_X2_MONEY,
    LB_SPAWN_ZOMBIES
};

struct LuckyBox {
    float x;
    float y;
    float speed;
    float baseY;
    float angle;
    float angularSpeed;
    bool alive;
    SDL_Texture* texture;
    LuckyBoxEffect effect;
    Uint32 effectStartTime;
    bool effectActive;

    LuckyBox(float _x, float _y, float _speed, SDL_Texture* _texture, LuckyBoxEffect _effect)
        : x(_x), y(_y), speed(_speed), baseY(_y), angle(0), angularSpeed(0.045f),
          alive(true), texture(_texture), effect(_effect), effectActive(false) {}

    void update() {
        x += speed;

        angle += angularSpeed;
        if (angle > 2 * M_PI) angle -= 2 * M_PI;

        float amplitude = 30.0f;
        y = baseY + amplitude * sin(angle);

        if (x > SCREEN_WIDTH + 50) alive = false;
    }

    void render(Graphics& graphics) {
        graphics.renderTexture(texture, int(x), int(y), 3);
    }

    SDL_Rect getRect() const {
        return SDL_Rect{ int(x), int(y), 48, 48 };
    }
};

#endif // _LUCKYBOX__H
