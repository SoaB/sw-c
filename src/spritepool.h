#ifndef __SPRITEPOOL_H__
#define __SPRITEPOOL_H__

#include "sprite.h"

typedef enum {
    SPR_PLAYER,
    SPR_ENEMY,
    SPR_EXPLODE,
    SPR_NUMS,
} SpriteType;

void SpritePoolInit();
Sprite* SpritePoolGet(SpriteType st);
void SpritePoolDestroy();
void SpritePoolUpdate();

#endif