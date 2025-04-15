#include "spritepool.h"
#include "commdef.h"
#include "sprite.h"
#include <string.h>

typedef struct {
    AnimFrame afEnemy[2];
    AnimFrame afExplod;
    Sprite enemy[2];
    Sprite explod;
} SprPool;

SprPool sp;
void SpritePoolInit()
{
    sp.afEnemy[0] = AnimFrameLoad("asset/SlimeIdle.png", 32, 32);
    sp.afEnemy[1] = AnimFrameLoad("asset/SlimeHurt.png", 32, 32);
    sp.afExplod = AnimFrameLoad("asset/explod.png", 32, 32);
    SpriteLoad(&sp.enemy[0], sp.afEnemy[0]);
    SpriteLoad(&sp.enemy[1], sp.afEnemy[1]);
    SpriteLoad(&sp.explod, sp.afExplod);
}

Sprite* SpritePoolGet(SpriteType st, int index)
{
    switch (st) {
    case SPR_ENEMY:
        return &sp.enemy[index];
    case SPR_EXPLODE:
        return &sp.explod;
    default:
        return NULL;
    }
}
void SpritePoolDestroy()
{
    AnimFrameUnload(&sp.afExplod);
    AnimFrameUnload(&sp.afEnemy[0]);
    AnimFrameUnload(&sp.afEnemy[1]);
}

void SpritePoolUpdate()
{
    SpriteUpdate(&sp.enemy[0]);
    SpriteUpdate(&sp.enemy[1]);
    SpriteUpdate(&sp.explod);
}