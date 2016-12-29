//
//  main.c
//  SAM
//
//  Created by Syed Ali on 2016-12-27.
//  Copyright © 2016 Syed Ali. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include "main.h"
#include "GameStatus.h"
#define GRAVITY 0.1f

int eventHandle(SDL_Window *window, GameState *gstate){
    SDL_Event event;
    int quit = 0;
    while(SDL_PollEvent(&event)){
        switch (event.type) {
            case SDL_WINDOWEVENT_CLOSE:
                    if(window){
                        SDL_DestroyWindow(window);
                        window = NULL;
                        quit = 1;
                        break;
                    }
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        SDL_DestroyWindow(window);
                        window = NULL;
                        quit = 1;
                        break;
                    case SDLK_UP:
                        if (gstate->mortyPosition.onLedge) {
                            gstate->mortyPosition.dy = -5;
                            gstate->mortyPosition.onLedge = 0;
                        }
                        break;
                }
                break;
            
            case SDL_QUIT:
                SDL_DestroyWindow(window);
                window = NULL;
                quit = 1;
                break;
                
        }
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_LEFT]){
            gstate->mortyPosition.facingLeft = 1;
            gstate->mortyPosition.dx -= 0.5;
            gstate->mortyPosition.slow = 0;
            if (gstate->mortyPosition.dx < -5) {
                gstate->mortyPosition.dx = 6;
            }
        }else if (state[SDL_SCANCODE_RIGHT]){
            gstate->mortyPosition.facingLeft = 0;
            gstate->mortyPosition.slow = 0;
            gstate->mortyPosition.dx += 0.5;
            if (gstate->mortyPosition.dx > 6) {
                gstate->mortyPosition.dx = 6;
            }
        }
        //friction
        else{
            gstate->mortyPosition.animationFrame = 0;
            gstate->mortyPosition.slow = 1;
            gstate->mortyPosition.dx *= 0.24f;
            if (fabs(gstate->mortyPosition.dx) < 0.2f) {
                gstate->mortyPosition.dx = 0;
            }
        }
    }
    return quit;
}

void renderGrpahics(GameState *gstate){
    if (gstate->displayStatusScreen == STATUS_DISPLAY_LIVES) {
        draw_lives(gstate);
    }else if (gstate->displayStatusScreen == STATUS_DISPLAY_GAME){
        SDL_SetRenderDrawColor(gstate->renderer, 172, 184, 247, 255);
        SDL_RenderClear(gstate->renderer);
        for (int i=0; i<100; i++) {
            SDL_Rect ledgeRect ={gstate->scrollX + gstate->ledges[i].x, gstate->ledges[i].y, gstate->ledges[i].w, gstate->ledges[i].h};
            SDL_RenderCopy(gstate->renderer, gstate->brick, NULL, &ledgeRect);
        }
        SDL_Rect mortyrect = {gstate->scrollX + gstate->mortyPosition.x, gstate->mortyPosition.y, 50, 68};
        SDL_RenderCopyEx(gstate->renderer, gstate->morty[gstate->mortyPosition.animationFrame], NULL, &mortyrect,0, NULL, gstate->mortyPosition.facingLeft == 0);
    }
    SDL_RenderPresent(gstate->renderer);
}

void processVelocity(GameState *gstate){
    gstate->time++;
    if (gstate->time > 120) {
        destroy_lives(gstate);
        gstate->displayStatusScreen = STATUS_DISPLAY_GAME;
    }
    if (gstate->displayStatusScreen == STATUS_DISPLAY_GAME) {
        gstate->mortyPosition.y += gstate->mortyPosition.dy;
        gstate->mortyPosition.x += gstate->mortyPosition.dx;
        gstate->mortyPosition.dy += GRAVITY;
        gstate->scrollX = -gstate->mortyPosition.x + 320;
        if (gstate->scrollX > 0) {
            gstate->scrollX = 0;
        }
        if (fabs(gstate->mortyPosition.dx) > 0 && gstate->mortyPosition.onLedge && !gstate->mortyPosition.   slow) {
            if (gstate->time%5 == 0) {
                if (gstate->mortyPosition.animationFrame == 0) {
                    gstate->mortyPosition.animationFrame = 1;
                }else {
                    gstate->mortyPosition.animationFrame = 0;
                }
            }
        }
    }
}

void hitDetection(GameState *gstate){
    for (int i=0; i<100; i++) {
        float mortyWidth = 50;
        float mortyHeight = 68;
        float mortyX = gstate->mortyPosition.x;
        float mortyY = gstate->mortyPosition.y;
        float brickX = gstate->ledges[i].x;
        float brickY = gstate->ledges[i].y;
        float brickWidth = gstate->ledges[i].w;
        float brickHeight = gstate->ledges[i].h;
        
        if (mortyWidth/2 +  mortyX > brickX && mortyX + mortyWidth/2 < brickX + brickWidth) {
            //hitting bottom of brick
            if (mortyY < brickY + brickHeight && mortyY > brickY && gstate->mortyPosition.dy < 0) {
                gstate->mortyPosition.y = brickY + brickHeight;
                mortyY = brickY + brickHeight;
                
                gstate->mortyPosition.dy = 0;
                gstate->mortyPosition.onLedge = 1;
            }
        }
        if (mortyX + mortyWidth > brickX && mortyX < brickX + brickWidth) {
            //hitting top of brick
            if (mortyY + mortyHeight > brickY && mortyY < brickY && gstate->mortyPosition.dy > 0) {
                gstate->mortyPosition.y = brickY - mortyHeight;
                mortyY = brickY - mortyHeight;
                
                gstate->mortyPosition.dy = 0;
                gstate->mortyPosition.onLedge = 1;
            }
        }
        if (mortyHeight + mortyY > brickY && mortyY < brickY + brickHeight) {
            //on right edge of brick
            if (mortyX < brickX + brickWidth && mortyX + mortyWidth > brickWidth + brickX && gstate->mortyPosition.dx < 0) {
                gstate->mortyPosition.x = brickX + brickWidth;
                mortyX = brickX + brickWidth;
                
                gstate->mortyPosition.dx = 0;
            }
            //on left edge of brick
            else if (mortyX + mortyWidth > brickX && mortyX < brickX && gstate->mortyPosition.dx > 0){
                gstate->mortyPosition.x = brickX - mortyWidth;
                mortyX = brickX - mortyWidth;
                
                gstate->mortyPosition.dx = 0;
            }
        }
    }
}

void loadGame(GameState *gstate){
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_Surface *mortySurface = NULL;
    SDL_Surface *mortyWalkSurface = NULL;
    SDL_Surface *brickSurface = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Window *window = NULL;
    window = SDL_CreateWindow("SAM", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    gstate->renderer = renderer;
    mortySurface = IMG_Load("/Users/Ali/Desktop/Learning_C/AlisProgram/Sources/SAM/SAM/Assets/Morty.png");
    mortyWalkSurface = IMG_Load("/Users/Ali/Desktop/Learning_C/AlisProgram/Sources/SAM/SAM/Assets/MortyWalk.png");
    brickSurface = IMG_Load("/Users/Ali/Desktop/Learning_C/AlisProgram/Sources/SAM/SAM/Assets/Ground.png");
    
    if (mortySurface == NULL) {
        printf("fcould not find image \n");
        exit(1);
    }
    gstate->font = TTF_OpenFont("/Users/Ali/Desktop/Learning_C/AlisProgram/Sources/SAM/SAM/Assets/MISFITS_.TTF", 48);
    if (!gstate->font) {
        printf("font could not be loaded \n");
        exit(1);
    }
    gstate->label = NULL;
    gstate->mortyPosition.lives = 3;
    gstate->displayStatusScreen = STATUS_DISPLAY_LIVES;
    init_lives(gstate);
    gstate->morty[0] = SDL_CreateTextureFromSurface(gstate->renderer, mortySurface);
    SDL_FreeSurface(mortySurface);
    gstate->morty[1] = SDL_CreateTextureFromSurface(gstate->renderer, mortyWalkSurface);
    SDL_FreeSurface(mortyWalkSurface);
    gstate->brick = SDL_CreateTextureFromSurface(gstate->renderer, brickSurface);
    SDL_FreeSurface(brickSurface);
    
    gstate->time = 0;
    gstate->scrollX = 0;
    gstate->mortyPosition.x = 220;
    gstate->mortyPosition.y = 140;
    gstate->mortyPosition.dx = 0;
    gstate->mortyPosition.dy = 0;
    gstate->mortyPosition.onLedge = 0;
    gstate->mortyPosition.animationFrame = 0;
    gstate->mortyPosition.facingLeft = 1;
    gstate->mortyPosition.slow = 1;
    
    for (int i=0; i<100; i++) {
        gstate->ledges[i].w = 256;
        gstate->ledges[i].h = 64;
        gstate->ledges[i].x = i*256;
        gstate->ledges[i].y = 400;
    }
    gstate->ledges[99].x = 350;
    gstate->ledges[99].y = 200;
    
    int quit = 0;
    while (!quit) {
        quit = eventHandle(window, gstate);
        renderGrpahics(gstate);
        processVelocity(gstate);
        hitDetection(gstate);
    }
    if (gstate->label) {
        TTF_CloseFont(gstate->font);
    }
    TTF_Quit();
    SDL_DestroyTexture(gstate->morty[0]);
    SDL_DestroyTexture(gstate->morty[1]);
    SDL_DestroyRenderer(gstate->renderer);
    SDL_DestroyWindow(gstate->window);
    SDL_Quit();
    
}



int main(int argc, const char * argv[]) {
    GameState gstate;
    loadGame(&gstate);
    return 0;
}
