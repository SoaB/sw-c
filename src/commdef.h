#ifndef __COMMDEF_H__
#define __COMMDEF_H__

#include "raylib.h"
#include "stdbool.h"
#include "stdint.h"

typedef Vector2 Vec2;
typedef Rectangle Rect;

#define CELL_SIZE 32
#define CELL_NUMS 20
#define SCR_WIDTH (CELL_SIZE * CELL_NUMS)
#define SCR_HEIGHT (CELL_SIZE * CELL_NUMS)
#define FPS 60

#define MAX_ENEMY 1000
#define ENEMY_HP 20
#define ENEMY_SPAWN_INTERVAL 0.5F

#endif