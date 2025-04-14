#ifndef __ENEMY_H__
#define __ENEMY_H__
#include "commdef.h"
#include "sprite.h"

typedef enum {
    ENEMY_NONE = 0,
    ENEMY_SLIME,
} EnemyType;

typedef struct {
    EnemyType type;
    Vec2 pos;
    int hp;
    float speed;
    float radius;
    float damage;
} Enemy;

void EnemyInit();
void TryAddEnemy(EnemyType eType);
void EnemySpawn();
void EnemyUpdate();
void EnemyDraw();

#endif