#include "spritepool.h"
#include "commdef.h"
#include "sprite.h"
#include <string.h>

typedef struct {
    AnimFrame afEnemy;
    AnimFrame afExplod;
    Sprite enemy;
    Sprite explod;
} SprPool;

SprPool sp;
void SpritePoolInit()
{
    sp.afEnemy = AnimFrameLoad("asset/SlimeIdle.png", 32, 32);
    sp.afExplod = AnimFrameLoad("asset/explod.png", 32, 32);
    SpriteLoad(&sp.enemy, sp.afEnemy);
    SpriteLoad(&sp.explod, sp.afExplod);
}

Sprite* SpritePoolGet(SpriteType st)
{
    switch (st) {
    case SPR_ENEMY:
        return &sp.enemy;
    case SPR_EXPLODE:
        return &sp.explod;
    default:
        return NULL;
    }
}
void SpritePoolDestroy()
{
    AnimFrameUnload(&sp.afExplod);
    AnimFrameUnload(&sp.afEnemy);
}

void SpritePoolUpdate()
{
    SpriteUpdate(&sp.enemy);
    SpriteUpdate(&sp.explod);
}