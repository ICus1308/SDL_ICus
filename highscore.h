#ifndef _HIGHSCORE__H
#define _HIGHSCORE__H

#include <bits/stdc++.h>
#include "variable.h"
#include "graphics.h"

const int baseY = 250;

vector <int> listScore;

void renderListHighScore() {
    graphics.prepareScene(menuBackground.texture);

    graphics.loadFont("Playthings.ttf", 48);
    SDL_Texture* titleTex = graphics.renderText("RANK", {255, 255, 255, 255});
    SDL_Rect titleRect; SDL_QueryTexture(titleTex, nullptr, nullptr, &titleRect.w, &titleRect.h);
    titleRect.x = (SCREEN_WIDTH - titleRect.w)/2;
    titleRect.y = 80;
    SDL_RenderCopy(graphics.renderer, titleTex, nullptr, &titleRect);
    SDL_DestroyTexture(titleTex);

    for (int i = 0; i < listScore.size(); i++) {
        string cur = "Top " + to_string(i + 1) + ": " + to_string(listScore[i]);
        SDL_Texture* scoreTex = graphics.renderText(cur.c_str(), {255, 255, 255, 255});
        SDL_Rect scoreRect; SDL_QueryTexture(scoreTex, nullptr, nullptr, &scoreRect.w, &scoreRect.h);
        scoreRect.x = (SCREEN_WIDTH - scoreRect.w)/2;
        scoreRect.y = baseY + i * 50;
        SDL_RenderCopy(graphics.renderer, scoreTex, nullptr, &scoreRect);
    }
}

void highScoreInit() {
    ifstream inp("highscore.txt");
    int score;
    listScore.clear();
    while (inp >> score)
        listScore.push_back(score);

    inp.close();
}

void highScoreLoop() {
    bool inListScore = true;
    while (inListScore) {
        renderListHighScore();
        graphics.presentScene();

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false, inListScore = false;

            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
                inListScore = false;
        }
    }
}

void saveNewScore() {
    listScore.clear();
    ifstream inp("highscore.txt");
    int score;
    while (inp >> score)
        listScore.push_back(score);
    inp.close();

    listScore.push_back(kills);
    ofstream out("highscore.txt");
    sort (listScore.rbegin(), listScore.rend());

    if (listScore.size() > 5)
        listScore.pop_back();

    for (int x : listScore)
        out << x << " ";
    out.close();
}

#endif // _HIGHSCORE__H
