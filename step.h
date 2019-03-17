#ifndef STEP_H
#define STEP_H
#include "particle_data.h"
void acc(particle_data *r, double mass, particle_data **planet);
void step(particle_data **r, int s, double step_size,  particle_data **q, double mass=1.989*std::pow(10,30));
void vel(particle_data *r, double t, double mass, particle_data **q);
void dist(particle_data *r,double t);

#endif	