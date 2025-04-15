#ifndef __ENEMY_H__
#define __ENEMY_H__
#include "commdef.h"
#include "sprite.h"

typedef enum {
    ENEMY_NONE = 0,
    ENEMY_SLIME,
} EnemyType;

typedef enum {
    ENEMY_GOOD = 0,
    ENEMY_DAMAGE,
    ENEMY_DIE,
} EnemyStatus;

typedef struct {
    EnemyType type;
    EnemyStatus status;
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