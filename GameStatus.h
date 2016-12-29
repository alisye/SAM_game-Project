//
//  GameStatus.h
//  SAM
//
//  Created by Syed Ali on 2016-12-28.
//  Copyright © 2016 Syed Ali. All rights reserved.
//

#ifndef GameStatus_h
#define GameStatus_h
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

void init_lives(GameState *gstate);
void draw_lives(GameState *gstate);
void destroy_lives(GameState *gstate);

#endif /* GameStatus_h */
