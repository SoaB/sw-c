#include "commdef.h"
#include "game.h"
#include "raylib.h"
#include "slime.h"
#include "timer.h"
#include <stdio.h>

Game game = { 0 };

int main()
{
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(SCR_WIDTH, SCR_HEIGHT, "Slime War");
    SetTargetFPS(FPS);
    GameInit(&game);
    while (!WindowShouldClose()) {
        GameUpdate(&game);
        BeginDrawing();
        ClearBackground((Color) { 0x18, 0x18, 0x18, 0xFF });
        GameDraw(&game);
        EndDrawing();
    }
    GameDestroy(&game);
    CloseWindow();
    return 0;
}