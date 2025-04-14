#include "player.h"
#include "raylib.h"
#include "raymath.h"
#include "timer.h"

typedef struct {
    Vec2 position;
    Vec2 currPos;
    int hp;
    int attackRange;
    int damage;
    float speed;
    float collisionRadius;
} Player;

Player player;

void PlayerInit(Vec2 pos, float speed, int hp)
{
    player.position = pos;
    player.currPos = pos;
    player.speed = speed;
    player.hp = hp;
    player.attackRange = 3;
    player.collisionRadius = 0.5F;
    player.damage = 1;
}

void PlayerDeInit()
{
}
inline Vec2 PlayerPosition()
{
    return player.position;
}
inline float PlayerColliRadius()
{
    return player.collisionRadius;
}
inline int PlayerAttackDamage()
{
    return player.attackRange;
}

void PlayerUpdate()
{
    float dt = gTimer.DeltaTime();
    if (IsKeyDown(KEY_W)) {
        if (player.position.y > 0) {
            player.position.y -= player.speed * dt;
        }
    }
    if (IsKeyDown(KEY_S)) {
        if (player.position.y < CELL_NUMS - 1) {
            player.position.y += player.speed * dt;
        }
    }
    if (IsKeyDown(KEY_A)) {
        if (player.position.x > 0) {
            player.position.x -= player.speed * dt;
        }
    }
    if (IsKeyDown(KEY_D)) {
        if (player.position.x < CELL_NUMS - 1) {
            player.position.x += player.speed * dt;
        }
    }
}

void PlayerDraw()
{
    Vec2 trPos = Vector2Scale(player.position, CELL_SIZE);
    DrawRectangleV(trPos, (Vec2) { CELL_SIZE, CELL_SIZE }, RED);
}