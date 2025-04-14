#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "commdef.h"

#define PARTICLE_NUM 20
#define PARTICLE_TIME 60

typedef struct {
    Vector2 pos[PARTICLE_NUM];
    Vector2 vel[PARTICLE_NUM];
    Color color[PARTICLE_NUM];
    int lifeTime;
} Particle;

void ParticleInit(Particle* e);
void ParticleStart(Particle* e, Vec2 target);
void ParticleUpdate(Particle* e);
void ParticleDraw(Particle* e);
#endif