#include "enemy.h"
#include "commdef.h"
#include "explod.h"
#include "player.h"
#include "raylib.h"
#include "raymath.h"
#include "sprite.h"
#include "timer.h"
#include "weapon.h"
#include <stdio.h>

static Enemy enemies[MAX_ENEMY];
static int enemyCounter;
static float enemySpawnTime = 0;
static Explod explods[MAX_ENEMY];
static int explodCounter;

void TryAddExplod(Vec2 pos)
{
    Vec2 sp = pos;
    sp = Vector2Scale(sp, CELL_SIZE);
    sp = Vector2AddValue(sp, (float)CELL_SIZE / 2);
    for (int i = 0; i < MAX_ENEMY; i++) {
        if (explods[i].lifeTime <= 0) {
            ExplodStart(&explods[i], sp);
            if (i >= explodCounter) {
                explodCounter++;
            }
            return;
        }
    }
}
void ExplodsUpdate()
{
    for (int i = 0; i < explodCounter; i++) {
        if (explods[i].lifeTime > 0) {
            ExplodUpdate(&explods[i]);
        }
    }
}
void ExplodsDraw()
{
    for (int i = 0; i < explodCounter; i++) {
        if (explods[i].lifeTime > 0) {
            ExplodDraw(&explods[i]);
        }
    }
}
void EnemyInit()
{
    for (int i = 0; i < MAX_ENEMY; ++i) {
        enemies[i].type = ENEMY_NONE;
        enemies[i].hp = ENEMY_HP;
        enemies[i].damage = 1;
        enemies[i].speed = 5;
        enemies[i].pos = (Vec2) { 0, 0 };
        enemies[i].radius = 0.5F;
        ExplodInit(&explods[i]);
    }
    enemyCounter = 0;
    enemySpawnTime = 0;
    explodCounter = 0;
}

void TryAddEnemy(EnemyType eType)
{
    for (int i = 0; i < MAX_ENEMY; ++i) {
        if (enemies[i].type == ENEMY_NONE) {
            enemies[i].type = eType;
            enemies[i].pos = (Vec2) { GetRandomValue(0, 19), GetRandomValue(0, 19) };
            enemies[i].hp = ENEMY_HP;
            enemyCounter += 1;
            printf("enemy counter : %d\n", enemyCounter);
            return;
        }
    }
}
void RemoveEnemy(int index)
{
    if (index >= MAX_ENEMY) {
        return;
    }
    if (enemies[index].type != ENEMY_NONE) {
        enemies[index] = enemies[enemyCounter - 1];
        enemies[enemyCounter - 1].type = ENEMY_NONE;
        enemyCounter -= 1;
    }
}
void EnemyAlife(int index)
{
    if (index >= MAX_ENEMY) {
        return;
    }
    int damage = PlayerAttackDamage();
    enemies[index].hp -= damage;
    if (enemies[index].hp <= 0) {
        TryAddExplod(enemies[index].pos);
        RemoveEnemy(index);
    }
}
void EnemySpawn()
{
    enemySpawnTime += gTimer.DeltaTime();
    if (enemySpawnTime >= ENEMY_SPAWN_INTERVAL) {
        enemySpawnTime = 0;
        TryAddEnemy(ENEMY_SLIME);
    }
}
void EnemyCollision()
{
    for (int i = 0; i < enemyCounter - 1; i++) {
        Enemy* enemyA = &enemies[i];
        if (enemyA->type == ENEMY_NONE) {
            continue;
        }
        for (int j = i + 1; j < enemyCounter; j++) {
            Enemy* enemyB = &enemies[j];
            if (enemyB->type == ENEMY_NONE) {
                continue;
            }
            float distanceSqr = Vector2DistanceSqr(enemyA->pos, enemyB->pos);
            float radiusA = enemyA->radius;
            float radiusB = enemyB->radius;
            float radiusSum = radiusA + radiusB;
            if (distanceSqr < radiusSum * radiusSum && distanceSqr > 0.001F) {
                float distance = sqrtf(distanceSqr);
                float overlap = radiusSum - distance;

                float positionCorrection = overlap / 5.0F;
                Vector2 direction = (Vector2) {
                    (enemyB->pos.x - enemyA->pos.x) / distance * positionCorrection,
                    (enemyB->pos.y - enemyA->pos.y) / distance * positionCorrection
                };
                enemyA->pos = Vector2Subtract(enemyA->pos, direction);
                enemyB->pos = Vector2Add(enemyB->pos, direction);
            }
        }
    }
}
void EnemyTouchPlayer()
{
    Vec2 playerPos = PlayerPosition();
    float playerRadius = PlayerColliRadius();
    float weaponRadius = WeaponAttackRange();
    for (int i = 0; i < enemyCounter; i++) {
        Enemy* enemy = &enemies[i];
        if (enemy->type == ENEMY_NONE) {
            continue;
        }
        float distanceSqr = Vector2DistanceSqr(playerPos, enemy->pos);
        // weapon
        float radiusSum = weaponRadius + enemy->radius;
        if (distanceSqr < radiusSum * radiusSum && distanceSqr > 0.001F) {
            // weapon hit
            EnemyAlife(i);
            //            printf("weapon attacked!!\n");
        }
        // player
        radiusSum = playerRadius + enemy->radius;
        if (distanceSqr < radiusSum * radiusSum && distanceSqr > 0.001F) {
            // Hit player !!!!
            EnemyAlife(i);
            //            printf("enemy counter : %d\n", enemyCounter);
        }
    }
}
void EnemyUpdate()
{
    Vec2 playerPos = PlayerPosition();
    float deltaTime = gTimer.DeltaTime();
    for (int i = 0; i < enemyCounter; i++) {
        if (enemies[i].type == ENEMY_NONE) {
            continue;
        }
        Vec2 dir = Vector2Subtract(playerPos, enemies[i].pos);
        dir = Vector2Normalize(dir);
        dir = Vector2Scale(dir, deltaTime * enemies[i].speed);
        enemies[i].pos = Vector2Add(enemies[i].pos, dir);
    }
    EnemyCollision();
    EnemyTouchPlayer();
    ExplodsUpdate();
}

void EnemyDraw(Sprite* spr)
{
    for (int i = 0; i < enemyCounter; i++) {
        if (enemies[i].type == ENEMY_NONE) {
            continue;
        }
        Vec2 trPos = Vector2Scale(enemies[i].pos, CELL_SIZE);
        SpriteDraw(spr, trPos);
    }
    ExplodsDraw();
}