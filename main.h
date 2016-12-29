//
//  main.h
//  SAM
//
//  Created by Syed Ali on 2016-12-28.
//  Copyright © 2016 Syed Ali. All rights reserved.
//

#ifndef main_h
#define main_h
#define STATUS_DISPLAY_LIVES 0
#define STATUS_DISPLAY_GAME 1
#define STATUS_DISPLAY_GAMEOVER 2
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>

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
    float dx;
    int animationFrame;
    int facingLeft;
    int onLedge;
    int slow;
    int lives;
}Position;

typedef struct GameState{
    Position mortyPosition;
    Ledge ledges[100];
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Texture *morty[2];
    SDL_Texture *brick;
    SDL_Texture *label;
    TTF_Font *font;
    int fontHeight;
    int fontWidth;
    int displayStatusScreen;
    int time;
    float scrollX;
}GameState;

#endif /* main_h */
