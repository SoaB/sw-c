#ifndef __GAME_H__
#define __GAME_H__

#include "commdef.h"
#include "enemy.h"
#include "player.h"
#include "sprite.h"
#include "timer.h"

typedef struct
{
    int mainStatus;
    int subStatus;
    AnimFrame af;
    Sprite slime;
} Game;
void GameInit(Game* game);
void GameDestroy(Game* game);
void GameUpdate(Game* game);
void GameDraw(Game* game);

#endif