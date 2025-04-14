#ifndef __EXPLOD_H__
#define __EXPLOD_H__

#include "commdef.h"

#define EXPLOD_PARTICLES 20
#define EXPLOD_TIME 60

typedef struct {
    Vector2 pos[EXPLOD_PARTICLES];
    Vector2 vel[EXPLOD_PARTICLES];
    Color color[EXPLOD_PARTICLES];
    int lifeTime;
} Explod;

void ExplodInit(Explod* e);
void ExplodStart(Explod* e, Vec2 target);
void ExplodUpdate(Explod* e);
void ExplodDraw(Explod* e);
#endif