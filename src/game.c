#include "game.h"
#include "enemy.h"
#include "player.h"
#include "raylib.h"
#include "sprite.h"
#include "spritepool.h"
#include "timer.h"
#include "weapon.h"
#include <stdlib.h>
#include <string.h>

void GameInit(Game* game)
{
    PlayerInit((Vec2) { 0, 0 }, 6, 100);
    EnemyInit();
    WeaponInit((Vec2) { 0, 0 });
    gTimer.Init();
    SpritePoolInit();
    TryAddEnemy(ENEMY_SLIME);
    TryAddEnemy(ENEMY_SLIME);
    TryAddEnemy(ENEMY_SLIME);
    TryAddEnemy(ENEMY_SLIME);
}

void GameDestroy(Game* game)
{
    SpritePoolDestroy();
}
void GameUpdate(Game* game)
{
    gTimer.Update();
    EnemySpawn();
    PlayerUpdate();
    EnemyUpdate();
    SpritePoolUpdate();
    WeasponUpdate(PlayerPosition());
}

void GameDraw(Game* game)
{
    PlayerDraw();
    EnemyDraw();
    WeaponDraw();
}
