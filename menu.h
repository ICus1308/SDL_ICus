#ifndef _MENU__H
#define _MENU__H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "defs.h"
#include <bits/stdc++.h>
#include "SDL_ttf.h"
#include "variable.h"
#include "shop.h"
#include "highscore.h"
#include "tutorial.h"
#include "music.h"

using namespace std;

vector<SDL_Texture*> textures;
vector<SDL_Rect> rects;
int selectedItem = 4;

const string title = "City's Corpse";
const vector <string> menuItems = {
    "Play",
    "Shop",
    "High Score",
    "Tutorial"
};

void initMenu() {
    menuBackground.setTexture(graphics.loadTexture(MENUBACKGROUND));
//    SDL_SetCursor(customCursorPointer);

    graphics.loadFont("Playthings.ttf", 48);
    SDL_Color color = { 255, 255, 255, 255 };

    for (size_t i = 0; i < menuItems.size(); ++i) {
        SDL_Texture *tex = graphics.renderText(menuItems[i].c_str(), color);

        textures.push_back(tex);

        SDL_Rect dst;
        SDL_QueryTexture(tex, nullptr, nullptr, &dst.w, &dst.h);
        dst.x = (SCREEN_WIDTH  - dst.w) / 2;
        dst.y = 350 + int(i) * (dst.h + 20);
        rects.push_back(dst);
    }

    graphics.loadFont("old_stamper.ttf", 72);
    SDL_Texture *text = graphics.renderText(title.c_str(), color);

    SDL_Rect dst;
    SDL_QueryTexture(text, nullptr, nullptr, &dst.w, &dst.h);
    dst.x = (SCREEN_WIDTH  - dst.w) / 2;
    dst.y = 200;

    textures.push_back(text);
    rects.push_back(dst);
}

int preSelectedShop = -1;

void renderOptions() {
    if (selectedItem == 4)
        preSelectedShop = -1;
    for (size_t i = 0; i < 4; ++i) {
        if (int(i) == selectedItem) {
            SDL_SetRenderDrawColor(graphics.renderer, 255, 255, 0, 255);
            SDL_Rect highlight = {
                rects[i].x - 10, rects[i].y - 5,
                rects[i].w + 20, rects[i].h + 10
            };
            SDL_RenderDrawRect(graphics.renderer, &highlight);
//            SDL_SetCursor(customCursorHover);
            if (preSelectedShop != selectedItem) {
                preSelectedShop = selectedItem;
                graphics.play(effectHover);
            }
        }
        SDL_RenderCopy(graphics.renderer, textures[i], nullptr, &rects[i]);
    }
}

void renderTitle() {
    SDL_RenderCopy(graphics.renderer, textures[4], nullptr, &rects[4]);
}

void menu() {
    while (running) {
        graphics.renderTexture(menuBackground.texture, 0, 0, 1);
//        graphics.renderBackground(background);
//        background.scroll(1);

        renderOptions();
        renderTitle();

        SDL_RenderPresent(graphics.renderer);

        while (SDL_PollEvent(&e) !=  0) {
            if (e.type == SDL_QUIT)
                running = false;

            else if (e.type == SDL_MOUSEMOTION) {
                int mx = e.motion.x, my = e.motion.y;
                bool checkInOptionMenu = false;
                for (size_t i = 0; i < rects.size(); ++i) {
                    if (mx >= rects[i].x && mx <= rects[i].x + rects[i].w &&
                        my >= rects[i].y && my <= rects[i].y + rects[i].h) {
                        selectedItem = int(i);
                        checkInOptionMenu = true;
                    }
                }

                if (checkInOptionMenu == false)
                    selectedItem = 4;
            }

            else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                switch (selectedItem) {
                    case 0:
                        gameInit();
                        gamePlay();
                        gameOut();
                        break;
                    case 1:
                        shopLoop();
                        break;
                    case 2:
                        highScoreInit();
                        highScoreLoop();
                        break;
                    case 3:
                        tutorialLoop();
                        break;
                }
            }
        }

        SDL_Delay(20);
    }
}

void menuOut() {
    SDL_FreeCursor(customCursorPointer);
    SDL_FreeCursor(customCursorHover);
    SDL_FreeSurface(cursorPointer);
    SDL_FreeSurface(cursorHover);
    for (auto tex : textures) SDL_DestroyTexture(tex);
}

#endif // _MENU__H
