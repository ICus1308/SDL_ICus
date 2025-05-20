#ifndef _SHOP__H
#define _SHOP__H

#include "graphics.h"
#include "variable.h"

struct ShopItem {
    string name;
    int price;
    SDL_Texture* texture;
    SDL_Rect rect;
    bool bought = false;
};
vector<ShopItem> shopItems;

int shopSelected = -1;
int shopClicked = -1;
bool inShop = false;

SDL_Rect buyRect = {SCREEN_WIDTH - 250, SCREEN_HEIGHT - 100, 160, 60};
bool buyHovered = false;

void initShop() {
    shopItems.clear();
    shopItems.push_back({"Piercing", 1000, graphics.loadTexture(SKILL1), {220, 250, 96, 96}, false});
    shopItems.push_back({"Shock Wave", 1500, graphics.loadTexture(SKILL2), {220, 380, 96, 96}, false});

    ifstream inp("shopbought.txt");
    for (int i = 0; i < 2; i++)
        inp >> shopItems[i].bought;
    inp.close();
}

int preSelectedItem = -1;

void renderShop() {
    graphics.prepareScene(menuBackground.texture);

    SDL_Color color = {255, 255, 255, 0};

    graphics.loadFont("Playthings.ttf", 48);
    SDL_Texture* titleTex = graphics.renderText("SHOP", color);
    SDL_Rect titleRect; SDL_QueryTexture(titleTex, nullptr, nullptr, &titleRect.w, &titleRect.h);
    titleRect.x = (SCREEN_WIDTH - titleRect.w)/2;
    titleRect.y = 80;
    SDL_RenderCopy(graphics.renderer, titleTex, nullptr, &titleRect);
    SDL_DestroyTexture(titleTex);

    SDL_Texture *curCoins = graphics.renderText(("Coins: " + to_string(coins)).c_str(), color);
    graphics.renderTexture(curCoins, 10, 10, 1);
    SDL_DestroyTexture(curCoins);

//    graphics.loadFont("DroplineRegular-Wpegz.otf", 24);
//    SDL_Texture* wayOut = graphics.renderText("Press ESC to exit", {255, 215, 0, 255});
//    graphics.renderTexture(wayOut, 10, 650, 1);
//    SDL_DestroyTexture(wayOut);

    if (shopSelected == -1)
        preSelectedItem = -1;

    for (int i = 0; i < shopItems.size(); ++i) {
        if (shopSelected == i) {
            SDL_SetRenderDrawColor(graphics.renderer, 255, 255, 0, 255);
            SDL_Rect hl = shopItems[i].rect;
            hl.x -= 8; hl.y -= 8; hl.w += 16; hl.h += 16;
            SDL_RenderDrawRect(graphics.renderer, &hl);
            if (preSelectedItem != shopSelected) {
                preSelectedItem = shopSelected;
                graphics.play(effectHover);
            }
        }
        if (shopClicked == i) {
            SDL_SetRenderDrawColor(graphics.renderer, 0, 255, 255, 255);
            SDL_Rect hl = shopItems[i].rect;
            hl.x -= 12; hl.y -= 12; hl.w += 24; hl.h += 24;
            SDL_RenderDrawRect(graphics.renderer, &hl);
        }

        SDL_RenderCopy(graphics.renderer, shopItems[i].texture, nullptr, &shopItems[i].rect);

        if (shopItems[i].bought) {
            graphics.loadFont("Playthings.ttf", 32);
            SDL_Texture* boughtTex = graphics.renderText("Bought", {0,255,0,255});
            SDL_Rect br; SDL_QueryTexture(boughtTex, nullptr, nullptr, &br.w, &br.h);
            br.x = shopItems[i].rect.x + shopItems[i].rect.w + 24;
            br.y = shopItems[i].rect.y + 40;
            SDL_RenderCopy(graphics.renderer, boughtTex, nullptr, &br);
            SDL_DestroyTexture(boughtTex);
        }
    }

    graphics.loadFont("Playthings.ttf", 24);
    SDL_Texture* infoSkill_1 = graphics.renderText("Piercing (passive): 1000 coins", color);
    SDL_Texture* infoSkill_2 = graphics.renderText("Shock Wave (active): 1500 coins", color);
    graphics.renderTexture(infoSkill_1, 340, 260, 1);
    graphics.renderTexture(infoSkill_2, 340, 390, 1);

    if (shopClicked != -1) {
        const ShopItem& item = shopItems[shopClicked];
        if (!item.bought) {
            SDL_SetRenderDrawColor(graphics.renderer, buyHovered && coins >= item.price ? 255 : 180, 255, buyHovered && coins >= item.price ? 0 : 180, 255);
            SDL_RenderFillRect(graphics.renderer, &buyRect);

            graphics.loadFont("Playthings.ttf", 36);
            SDL_Texture* buyTex = graphics.renderText("Buy", {0, 0, 0, 255});
            SDL_Rect br; SDL_QueryTexture(buyTex, nullptr, nullptr, &br.w, &br.h);
            br.x = buyRect.x + (buyRect.w - br.w)/2;
            br.y = buyRect.y + (buyRect.h - br.h)/2;
            SDL_RenderCopy(graphics.renderer, buyTex, nullptr, &br);
            SDL_DestroyTexture(buyTex);
        }
        else {
            SDL_SetRenderDrawColor(graphics.renderer, 150, 255, 150, 255);
            SDL_RenderFillRect(graphics.renderer, &buyRect);
            graphics.loadFont("Playthings.ttf", 36);
            SDL_Texture* boughtTex = graphics.renderText("Bought", {0, 100, 0, 255});
            SDL_Rect br; SDL_QueryTexture(boughtTex, nullptr, nullptr, &br.w, &br.h);
            br.x = buyRect.x + (buyRect.w - br.w)/2;
            br.y = buyRect.y + (buyRect.h - br.h)/2;
            SDL_RenderCopy(graphics.renderer, boughtTex, nullptr, &br);
            SDL_DestroyTexture(boughtTex);
        }
    }
}

void shopLoop() {
    inShop = true;
    shopClicked = -1;
    while (inShop) {
        renderShop();
        graphics.presentScene();

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false, inShop = false;

            else if (e.type == SDL_MOUSEMOTION) {
                int mx = e.motion.x, my = e.motion.y;
                shopSelected = -1;
                buyHovered = false;
                for (int i = 0; i < shopItems.size(); ++i) {
                    if (mx >= shopItems[i].rect.x && mx <= shopItems[i].rect.x + shopItems[i].rect.w &&
                        my >= shopItems[i].rect.y && my <= shopItems[i].rect.y + shopItems[i].rect.h)
                        shopSelected = i;
                }
                if (shopClicked != -1 && !shopItems[shopClicked].bought) {
                    if (mx >= buyRect.x && mx <= buyRect.x + buyRect.w &&
                        my >= buyRect.y && my <= buyRect.y + buyRect.h)
                        buyHovered = true;
                }
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                int mx = e.button.x, my = e.button.y;
                bool clickedSkill = false;
                for (int i = 0; i < shopItems.size(); ++i) {
                    if (mx >= shopItems[i].rect.x && mx <= shopItems[i].rect.x + shopItems[i].rect.w &&
                        my >= shopItems[i].rect.y && my <= shopItems[i].rect.y + shopItems[i].rect.h) {
                        shopClicked = i;
                        clickedSkill = true;
                        break;
                    }
                }
                if (!clickedSkill && shopClicked != -1 && !shopItems[shopClicked].bought &&
                    mx >= buyRect.x && mx <= buyRect.x + buyRect.w &&
                    my >= buyRect.y && my <= buyRect.y + buyRect.h &&
                    coins >= shopItems[shopClicked].price) {
                    coins -= shopItems[shopClicked].price;
                    shopItems[shopClicked].bought = true;
                    graphics.play(effectBuy);
                }

                else if (!clickedSkill && !(
                    mx >= buyRect.x && mx <= buyRect.x + buyRect.w &&
                    my >= buyRect.y && my <= buyRect.y + buyRect.h
                    )
                         )
                        shopClicked = -1;
            }
            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
                inShop = false;
        }
        SDL_Delay(20);
    }
}

void shopOut() {
    ofstream out("shopbought.txt");
    for (int i = 0; i < 2; i++)
        out << shopItems[i].bought << " ";
    out.close();
}

#endif // _SHOP__H
