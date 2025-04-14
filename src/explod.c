#include "explod.h"
#include "commdef.h"
#include "raylib.h"
#include "raymath.h"
#include "sprite.h"
#include "timer.h"
#include <math.h>

#define EXPLOD_TIME 60
void ExplodInit(Explod* e)
{
    e->pos = (Vec2) { 0, 0 };
    e->lifeTime = 0;
}

void ExplodStart(Explod* e, Vec2 target)
{
    e->pos = target;
    e->lifeTime = EXPLOD_TIME;
}
void ExplodUpdate(Explod* e)
{
    if (e->lifeTime <= 0) {
        return;
    }
    e->lifeTime -= 1;
}
void ExplodDraw(Explod* e, Sprite* s)
{
    if (e->lifeTime <= 0)
        return;
    SpriteDraw(s, e->pos);
}
