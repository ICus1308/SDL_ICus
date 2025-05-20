#ifndef _GRAPHICS__H
#define _GRAPHICS__H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "defs.h"
#include <bits/stdc++.h>
#include "SDL_ttf.h"
#include "SDL_mixer.h"

struct ScrollingBackground {
    SDL_Texture* texture;
    int scrollingOffset = 0;
    int width, height;

    void setTexture(SDL_Texture* _texture) {
        texture = _texture;
        SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    }

    void scroll(int distance) {
        scrollingOffset += distance;
        if( scrollingOffset > height ) { scrollingOffset = 0; }
    }
};

struct MenuBackground {
    SDL_Texture* texture;
    int width, height;

    void setTexture(SDL_Texture* _texture) {
        texture = _texture;
        SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    }
};

struct Crosshair {
    SDL_Texture* texture;
    int x, y;
    int width, height;
    int mouseX, mouseY;

    void setTexture(SDL_Texture* _texture) {
        texture = _texture;
        SDL_QueryTexture(texture, NULL, NULL, &width, &height);

        width /= 5;
        height /= 5;
    }

    void update() {
        SDL_GetMouseState(&mouseX, &mouseY);
    }

    void render(SDL_Renderer* renderer) {
        SDL_Rect rect = {
            static_cast <int> (mouseX - width / 2),
            static_cast <int> (mouseY - height / 2),
            width,
            height
        };

        SDL_RenderCopy(renderer, texture, NULL, &rect);
    }
};

struct Graphics {
    SDL_Renderer *renderer;
	SDL_Window *window;
	TTF_Font* font;

	void logErrorAndExit(const char* msg, const char* error)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
        SDL_Quit();
    }

	void init() {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
            logErrorAndExit("SDL_Init", SDL_GetError());

        window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        //full screen
        //window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
        if (window == nullptr) logErrorAndExit("CreateWindow", SDL_GetError());

        if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
            logErrorAndExit( "SDL_image error:", IMG_GetError());

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                              SDL_RENDERER_PRESENTVSYNC);
        //Khi chạy trong máy ảo (ví dụ phòng máy ở trường)
        //renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));

        if (renderer == nullptr) logErrorAndExit("CreateRenderer", SDL_GetError());

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

        if (TTF_Init() == -1) {
            logErrorAndExit("SDL_ttf could not initialize! SDL_ttf Error: ",
                             TTF_GetError());
        }

        if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
           logErrorAndExit( "SDL_mixer could not initialize! SDL_mixer Error: %s\n",
                            Mix_GetError() );
        }
    }

	void prepareScene(SDL_Texture * background = nullptr)
    {
        SDL_RenderClear(renderer);
        if (background != nullptr) SDL_RenderCopy( renderer, background, NULL, NULL);
    }

    void presentScene()
    {
        SDL_RenderPresent(renderer);
    }

    SDL_Texture *loadTexture(const char *filename)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

        SDL_Texture *texture = IMG_LoadTexture(renderer, filename);
        if (texture == NULL)
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Load texture %s", IMG_GetError());

        return texture;
    }

    void renderTexture(SDL_Texture *texture, int x, int y, int Ratio)
    {
        SDL_Rect dest;

        dest.x = x;
        dest.y = y;
        SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

        dest.h /= Ratio;
        dest.w /= Ratio;

        SDL_RenderCopy(renderer, texture, NULL, &dest);
    }

    void renderTextureEx(SDL_Texture *texture, int x, int y, int Ratio)
    {
        SDL_Rect dest;

        dest.x = x;
        dest.y = y;
        SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

        dest.h /= Ratio;
        dest.w /= Ratio;

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        int centerX = dest.w / 2;
        int centerY = dest.h * 3 / 4;

        SDL_Point Cen = {centerX, centerY};

        float dx = mouseX - SCREEN_WIDTH_CENTER;
        float dy = mouseY - (dest.y + dest.h * 3 / 4);
        float angle = 90 + atan2(dy, dx) * 180.0 / 3.14;

        SDL_RenderCopyEx(renderer, texture, NULL, &dest, angle, &Cen, SDL_FLIP_NONE);
    }

    void blitRect(SDL_Texture *texture, SDL_Rect *src, int x, int y)
    {
        SDL_Rect dest;

        dest.x = x;
        dest.y = y;
        dest.w = src->w;
        dest.h = src->h;

        SDL_RenderCopy(renderer, texture, src, &dest);
    }

    void renderBackground(const ScrollingBackground& background) {
        renderTexture(background.texture, 0, background.scrollingOffset, 1);
        renderTexture(background.texture, 0, background.scrollingOffset - background.height, 1);
    }

    void loadFont(const char* path, int size)
    {
        font = TTF_OpenFont( path, size );
        if (font == nullptr) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                           SDL_LOG_PRIORITY_ERROR,
                           "Load font %s", TTF_GetError());
        }
    }

     SDL_Texture* renderText(const char* text, SDL_Color textColor)
    {
        SDL_Surface* textSurface =
                TTF_RenderText_Solid( font, text, textColor );
        if( textSurface == nullptr ) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                           SDL_LOG_PRIORITY_ERROR,
                           "Render text surface %s", TTF_GetError());
            return nullptr;
        }

        SDL_Texture* texture =
                SDL_CreateTextureFromSurface( renderer, textSurface );
        if( texture == nullptr ) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                           SDL_LOG_PRIORITY_ERROR,
                           "Create texture from text %s", SDL_GetError());
        }
        SDL_FreeSurface( textSurface );
        return texture;
    }

    void drawCircle(int xc, int yc, int r) { // Midpoint circle algorithm
        int x = r;
        int y = 0;
        int err = 0;

        while (x >= y) {
            SDL_RenderDrawPoint(renderer, xc + x, yc + y);
            SDL_RenderDrawPoint(renderer, xc + y, yc + x);
            SDL_RenderDrawPoint(renderer, xc - y, yc + x);
            SDL_RenderDrawPoint(renderer, xc - x, yc + y);
            SDL_RenderDrawPoint(renderer, xc - x, yc - y);
            SDL_RenderDrawPoint(renderer, xc - y, yc - x);
            SDL_RenderDrawPoint(renderer, xc + y, yc - x);
            SDL_RenderDrawPoint(renderer, xc + x, yc - y);

            y++;
            err += 1 + 2*y;
            if (2*(err - x) + 1 > 0) {
                x--;
                err += 1 - 2*x;
            }
        }
    }

    Mix_Music *loadMusic(const char* path)
    {
        Mix_Music *gMusic = Mix_LoadMUS(path);
        if (gMusic == nullptr) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                           SDL_LOG_PRIORITY_ERROR,
                "Could not load music! SDL_mixer Error: %s", Mix_GetError());
        }
        return gMusic;
    }

    void play(Mix_Music *gMusic)
    {
        if (gMusic == nullptr) return;

        if (Mix_PlayingMusic() == 0) {
            Mix_PlayMusic( gMusic, -1 );
        }
        else if( Mix_PausedMusic() == 1 ) {
            Mix_ResumeMusic();
        }
    }

    Mix_Chunk* loadSound(const char* path) {
        Mix_Chunk* gChunk = Mix_LoadWAV(path);
        if (gChunk == nullptr) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                           SDL_LOG_PRIORITY_ERROR,
               "Could not load sound! SDL_mixer Error: %s", Mix_GetError());
        }
    }

    void play(Mix_Chunk* gChunk) {
        if (gChunk != nullptr) {
            Mix_PlayChannel( -1, gChunk, 0 );
        }
    }

    void quit()
    {
        Mix_Quit();

        IMG_Quit();

        TTF_CloseFont(font);
        TTF_Quit();

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
};

#endif // _GRAPHICS__H
