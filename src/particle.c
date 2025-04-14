#include "particle.h"
#include "commdef.h"
#include "raylib.h"
#include "timer.h"
#include <math.h>

void ParticleInit(Particle* e)
{
    for (int i = 0; i < PARTICLE_NUM; i++) {
        e->pos[i] = (Vec2) { 0, 0 };
        e->vel[i] = (Vec2) { 0, 0 };
        e->color[i] = (Color) { 0, 0, 0, 255 };
    }
    e->lifeTime = 0;
}

void ParticleStart(Particle* e, Vec2 target)
{
    float deltaTime = gTimer.DeltaTime();
    for (int i = 0; i < PARTICLE_NUM; i++) {
        e->pos[i] = target;
        float angle = (float)GetRandomValue(0, 360) * DEG2RAD; // 角度轉為弧度
        float speed = (float)GetRandomValue(100, 150) * deltaTime; // 速度範圍
        e->vel[i].x = cosf(angle) * speed;
        e->vel[i].y = sinf(angle) * speed;
        e->color[i] = (Color) { 255, GetRandomValue(100, 255), 0, 255 };
    }
    e->lifeTime = PARTICLE_TIME;
}
void ParticleUpdate(Particle* e)
{
    if (e->lifeTime <= 0)
        return;
    for (int i = 0; i < PARTICLE_NUM; i++) {
        e->pos[i].x += e->vel[i].x;
        e->pos[i].y += e->vel[i].y;
        e->vel[i].y += 0.1f;
    }
    e->lifeTime -= 1;
}
void ParticleDraw(Particle* e)
{
    if (e->lifeTime <= 0)
        return;
    for (int i = 0; i < PARTICLE_NUM; i++) {
        Color color = e->color[i];
        color.a = (unsigned char)((float)e->lifeTime / PARTICLE_TIME * 255);
        float r = e->lifeTime / 15.0;
        DrawCircleV(e->pos[i], r, color);
    }
}
