#ifndef _COIN__H
#define _COIN__H

#include <bits/stdc++.h>
#include "SDL2/SDL.h"
#include "SDL_ttf.h"
#include "variable.h"

void getNumberOfCoin() {
    ifstream inp("coin.txt");

    inp >> coins;

    inp.close();
}

void saveNewDataOfCoin() {
    ofstream out("coin.txt");

    out << coins;

    out.close();
}

#endif // _COIN__H
