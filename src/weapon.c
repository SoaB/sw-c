#include "weapon.h"
#include "commdef.h"
#include "raylib.h"
#include "raymath.h"
#include "timer.h"
#include <math.h>

typedef enum {
    WEAPON_NONE,
    WEAPON_RING_BULLET,
} WeaponType;

typedef struct {
    Vec2 pos[30];
    float size;
    float radius;
    float attackRange;
    float speed;
    float frameCount;
} RingBullet;

typedef struct {
    WeaponType type;
    void* weapon[2];
    int weaponCount;
} Weapon;

typedef struct {
    float sin[30];
    float cos[30];
} Lut;

Lut lut;
RingBullet rBullet;

void InitSCLut()
{
    for (int i = 0; i < 30; i++) {
        lut.sin[i] = sinf((float)i * DEG2RAD * 12);
        lut.cos[i] = cosf((float)i * DEG2RAD * 12);
    }
}

void WeaponInit(Vec2 pos)
{
    InitSCLut();
    for (int i = 0; i < 30; i++) {
        rBullet.pos[i] = pos;
    }
    rBullet.size = 2;
    rBullet.radius = 1.0F;
    rBullet.attackRange = 1.0F;
    rBullet.speed = 50.0F;
    rBullet.frameCount = 0;
}
float WeaponAttackRange()
{
    return rBullet.radius;
}
void WeasponUpdate(Vec2 pos)
{
    rBullet.frameCount += (gTimer.DeltaTime() * rBullet.speed);
    int findex = (int)floorf(rBullet.frameCount);
    float radius = rBullet.radius + (1.0F * lut.sin[findex % 30]);
    for (int i = 0; i < 30; i++) {
        rBullet.pos[i].x = pos.x + radius * lut.cos[i];
        rBullet.pos[i].y = pos.y + radius * lut.sin[i];
    }
}

void WeaponDraw()
{
    for (int i = 0; i < 30; i++) {
        Vec2 pos = Vector2Scale(rBullet.pos[i], CELL_SIZE);
        DrawCircleV(pos, rBullet.size, RED);
    }
}