#ifndef __WEAPON_H__
#define __WEAPON_H__
#include "commdef.h"

void WeaponInit(Vec2 pos);
void WeasponUpdate(Vec2 pos);
void WeaponDraw();
float WeaponAttackRange();
#endif