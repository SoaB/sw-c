#ifndef __EXPLOD_H__
#define __EXPLOD_H__

#include "commdef.h"
#include "sprite.h"

typedef struct {
    Vec2 pos;
    int lifeTime;
} Explod;

void ExplodInit(Explod* e);
void ExplodStart(Explod* e, Vec2 target);
void ExplodUpdate(Explod* e);
void ExplodDraw(Explod* e, Sprite* s);
#endif