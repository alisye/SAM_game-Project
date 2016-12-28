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
#define GRAVITY 0.06f


typedef struct Ledge{
    int x;
    int y;
    int w;
    int h;
}Ledge;

typedef struct Position{
    float x;
    float y;
    float dy;
    int onLedge;
}Position;

typedef struct GameState{
    Position mortyPosition;
    Ledge ledges[100];
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Texture *morty[2];
    SDL_Texture *brick;
}GameState;


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
            gstate->mortyPosition.x -= 8;
        }else if (state[SDL_SCANCODE_RIGHT]){
            gstate->mortyPosition.x += 8;
        }
    }
    return quit;
}

void renderGrpahics(GameState *gstate){
    
    SDL_SetRenderDrawColor(gstate->renderer, 0, 255, 0, 255);
    SDL_RenderClear(gstate->renderer);
    for (int i=0; i<100; i++) {
        SDL_Rect ledgeRect ={gstate->ledges[i].x, gstate->ledges[i].y, gstate->ledges[i].w, gstate->ledges[i].h};
        SDL_RenderCopy(gstate->renderer, gstate->brick, NULL, &ledgeRect);
    }
    SDL_Rect mortyrect = {gstate->mortyPosition.x, gstate->mortyPosition.y, 50, 68};
    SDL_RenderCopy(gstate->renderer, gstate->morty[0], NULL, &mortyrect);
    SDL_RenderPresent(gstate->renderer);

}

void processVelocity(GameState *gstate){
    gstate->mortyPosition.y += gstate->mortyPosition.dy;
    gstate->mortyPosition.dy += GRAVITY;
}

void hitDetection(GameState *gstate){
    for (int i=0; i<100; i++) {
        float mortyWidth = 50;
        float mortyHeight = 68;
        float mortyX = gstate->mortyPosition.x;
        float mortyY = gstate->mortyPosition.y;
        float birckX = gstate->ledges[i].x;
        float birckY = gstate->ledges[i].y;
        float birckWidth = gstate->ledges[i].w;
        float birckHeight = gstate->ledges[i].h;
        
        if (mortyHeight + mortyY > birckY && mortyY < birckY + birckHeight) {
            //right edge
            if (mortyX < birckX + birckWidth && mortyX + mortyWidth > birckX + birckWidth) {
                gstate->mortyPosition.x = birckX + birckWidth;
                mortyX = birckX + birckWidth;
            }
            //left edge
            else if (mortyX + mortyWidth > birckX && mortyX < birckX){
                gstate->mortyPosition.x = birckX - mortyWidth;
                mortyX = birckX - mortyWidth;
                
            }
        }
        if (mortyX + mortyWidth > birckX && mortyX < birckX + birckWidth) {
            //hitting top
            if (mortyY < birckY + birckHeight && mortyY > birckY) {
                gstate->mortyPosition.y = birckY + birckHeight;
                gstate->mortyPosition.dy = 0;
                gstate->mortyPosition.onLedge = 1;
                mortyY = birckX + birckHeight;
            }
            //landing
            else if (mortyY + mortyHeight > birckY && mortyY < birckY){
                gstate->mortyPosition.y = birckY - mortyHeight;
                gstate->mortyPosition.dy = 0;
                gstate->mortyPosition.onLedge = 1;
                mortyY = birckY - mortyHeight;
            }
        }
    }
    
}

void loadGame(GameState *gstate){
    SDL_Init(SDL_INIT_VIDEO);
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
    
    gstate->morty[0] = SDL_CreateTextureFromSurface(gstate->renderer, mortySurface);
    SDL_FreeSurface(mortySurface);
    gstate->morty[1] = SDL_CreateTextureFromSurface(gstate->renderer, mortyWalkSurface);
    SDL_FreeSurface(mortyWalkSurface);
    gstate->brick = SDL_CreateTextureFromSurface(gstate->renderer, brickSurface);
    SDL_FreeSurface(brickSurface);
    
    gstate->mortyPosition.x = 220;
    gstate->mortyPosition.y = 140;
    gstate->mortyPosition.dy = 0;
    gstate->mortyPosition.onLedge = 0;
    
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
