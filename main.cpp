#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "graphics.h"
#include "defs.h"
#include "handle.h"
#include "entities.h"
#include "game.h"
#include "menu.h"
#include "coin.h"
#include "shop.h"

using namespace std;

int main(int argc, char *argv[])
{
    graphics.init();
    getNumberOfCoin();
    loadAllSound();
    initShop();
    background.setTexture(graphics.loadTexture(BACKGROUND_IMG));

    graphics.play(backgroundMusic);

    initMenu();
    menu();

    menuOut();
    saveNewDataOfCoin();
    shopOut();
    graphics.quit();
    return 0;
}
