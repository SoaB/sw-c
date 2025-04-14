#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "commdef.h"

typedef struct AnimFrame {
    Texture2D tex;
    int cellW;
    int cellH;
    int centerW;
    int centerH;
    int xCellCount;
    int yCellCount;
} AnimFrame;

typedef struct {
    int id;
    AnimFrame af;
    int frame_counter;
    int curr_frame;
    float next_frame_time;
    float frame_time;
    bool animate;
    bool centor_cord;
} Sprite;

AnimFrame AnimFrameLoad(const char* fname, uint16_t cell_width, uint16_t cell_height);
void AnimFrameUnload(AnimFrame* af);
void SpriteLoad(Sprite* s, AnimFrame af);
void SpriteUpdate(Sprite* s);
void SpriteDraw(Sprite* s, Vector2 position);

#endif
