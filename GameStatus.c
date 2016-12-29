//
//  GameStatus.c
//  SAM
//
//  Created by Syed Ali on 2016-12-28.
//  Copyright © 2016 Syed Ali. All rights reserved.
//

#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

void init_lives(GameState *gstate){
    SDL_Color fontColor = {66, 244, 170, 255};
    char text[128] = "";
    sprintf(text, "Mick and Rorty LIVES x %d", gstate->mortyPosition.lives);
    SDL_Surface *width = TTF_RenderText_Blended(gstate->font, "Mick and Rorty", fontColor);
    SDL_Surface *fontSurface = TTF_RenderText_Blended_Wrapped(gstate->font, text, fontColor, width->w);
    SDL_FreeSurface(width);
    gstate->fontHeight = fontSurface->h;
    gstate->fontWidth = fontSurface->w;
    gstate->label = SDL_CreateTextureFromSurface(gstate->renderer, fontSurface);
    SDL_FreeSurface(fontSurface);
    }
void draw_lives(GameState *gstate){
    SDL_SetRenderDrawColor(gstate->renderer, 0, 0, 0, 255);
    SDL_RenderClear(gstate->renderer);
    SDL_Rect fontRect= {450 - gstate->fontWidth, 240 - gstate->fontHeight, gstate->fontWidth, gstate->fontHeight};
    SDL_RenderCopy(gstate->renderer, gstate->label, NULL, &fontRect);
}
void destroy_lives(GameState *gstate){
    SDL_DestroyTexture(gstate->label);
    gstate->label = NULL;
}
