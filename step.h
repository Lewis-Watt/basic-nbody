#ifndef STEP_H
#define STEP_H
#include "particle_data.h"
void acc(particle_data *r, particle_data **planet);
void step(particle_data **r, int s, double t,  particle_data **q);
void vel(particle_data *r, double t, particle_data **q);
double hill_radius(particle_data *q);
void remove_particles(particle_data **r, particle_data **q);
double dist_to_planet(particle_data *r, particle_data *q);
double vel_to_planet(particle_data *r, particle_data *q);
double escape_vel(particle_data *r, particle_data *q);
void select(particle_data *r, particle_data **q,double t);
void dist(particle_data *r,double t);

#endif	