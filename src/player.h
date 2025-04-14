#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "commdef.h"

void PlayerInit(Vec2 pos, float speed, int hp);
void PlayerDeInit();
void PlayerUpdate();
void PlayerDraw();
Vec2 PlayerPosition();
float PlayerColliRadius();
int PlayerAttackDamage();

#endif