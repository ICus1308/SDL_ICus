#ifndef _TUTORIAL__H
#define _TUTORIAL__H

#include <bits/stdc++.h>
#include "graphics.h"

using namespace std;

void tutorialLoop() {
    SDL_Texture* texture = graphics.loadTexture(TUTORIAL);
    bool inTutorial = true;
    while (inTutorial) {
        SDL_SetRenderDrawColor(graphics.renderer, 0, 0, 0, 0);
        graphics.prepareScene(texture);
        graphics.presentScene();

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false, inTutorial = false;

            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
                inTutorial = false;
        }
    }
    SDL_DestroyTexture(texture);
}

#endif // _TUTORIAL__H
