#include "sprite.h"
#include "raylib.h"
#include "timer.h"
#include <stdio.h>

AnimFrame AnimFrameLoad(const char* fname, uint16_t cell_width, uint16_t cell_height)
{
    AnimFrame a = { 0 };

    a.tex = LoadTexture(fname);
    if (a.tex.id == 0) {
        printf("Error: Failed to load texture from %s\n", fname);
        return a;
    }
    if (cell_width <= 0 || cell_height <= 0) {
        a.cellW = 0;
        a.cellH = 0;
    } else {
        a.cellW = cell_width;
        a.centerW = a.cellW / 2;
        a.cellH = cell_height;
        a.centerH = a.cellH / 2;
        a.xCellCount = a.tex.width / a.cellW;
        a.yCellCount = a.tex.height / a.cellH;
    }
    return a;
}

void AnimFrameUnload(AnimFrame* af)
{
    UnloadTexture(af->tex);
}

void SpriteLoad(Sprite* s, AnimFrame af)
{
    s->af = af;
    s->next_frame_time = 0.1F;
    s->centor_cord = true;
    s->curr_frame = 0;
    s->frame_counter = 0;
    s->animate = true;
}

void SpriteUpdate(Sprite* s)
{
    if (s->animate) {
        s->frame_time += gTimer.DeltaTime();
        if (s->frame_time >= s->next_frame_time) {
            s->frame_time = 0.0F;
            s->curr_frame++;
            if (s->curr_frame >= (s->af.xCellCount * s->af.yCellCount)) {
                s->curr_frame = 0;
            }
        }
    }
}

void SpriteDraw(Sprite* s, Vector2 position)
{
    if (s->af.tex.id > 0) {
        Rectangle sourceRec = {
            (float)(s->curr_frame % s->af.xCellCount) * (float)s->af.cellW,
            0,
            (float)s->af.cellW,
            (float)s->af.cellH
        };
        Rectangle destRec = {
            position.x,
            position.y,
            (float)s->af.cellW,
            (float)s->af.cellH
        };
        Vector2 origin = { 0, 0 };
        if (s->centor_cord) {
            origin = (Vector2) { (float)s->af.centerW, (float)s->af.centerH };
            destRec.x = position.x - (float)s->af.centerW;
            destRec.y = position.y - (float)s->af.centerH;
        }
        DrawTexturePro(s->af.tex, sourceRec, destRec, origin, 0.0F, WHITE);
    }
}
