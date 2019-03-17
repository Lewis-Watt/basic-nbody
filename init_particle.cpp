#include <iostream>
#include <vector>
#include <string>
#define _USE_MATH_DEFINES
#include <cmath>
#include <fstream>
#include <sstream>
#include "particle_data.h"
#include "init_particle.h"
#include "units.h"

void init_particle(particle_data **r,int num_of_objects)
{
	/*
	Gives initial values for the particle data. Will need to set values after this
	*/
	double radius = (sma*(1-(eccentricity*eccentricity)))/(1+(eccentricity*std::cos(true_anomaly)));
	std::cout << "Initial radius: " << radius << std::endl;
	for (int i=0; i<num_of_objects; i++)
	{
		r[i] = new particle_data;
		r[i]->pos.reserve(3);
		r[i]->vel.reserve(3);
		r[i]->a.reserve(3);
		r[i]->pos[0] = radius*std::cos(true_anomaly);
		r[i]->pos[1] = radius*std::sin(true_anomaly);
		r[i]->pos[2] = 0;
		r[i]->gravity = false;
		r[i]->id = i;
	}
}

void initial_orbit_velocity(particle_data *r)
{	
	double vdeg = (2*M_PI*sma*(1 + (eccentricity*std::cos(true_anomaly))))/(period*std::sqrt(1-(eccentricity*eccentricity)));
	double vrad = (2*M_PI*sma*eccentricity*std::sin(true_anomaly))/(period*std::sqrt(1-(eccentricity*eccentricity)));
	double lvx = (vrad*std::cos(true_anomaly)) - (vdeg*std::sin(true_anomaly));
	double lvy = (vrad*std::sin(true_anomaly)) + (vdeg*std::cos(true_anomaly));
	r->vel[0] = lvx;
	r->vel[1] = lvy;
	std::cout << "vx: " << r->vel[0] << std::endl;
	std::cout << "vy: " << r->vel[1] << std::endl;
	/*
	Need to revisit this. Works for circular orbits but doesn't account for anything in 
	the z axis. Also kinda forced the vel vector.
	*/
	r->vel[2] = 0;
	std::cout << "Vmag: "; 
	std::cout << std::sqrt((r->vel[0]*r->vel[0] + r->vel[1]*r->vel[1] + r->vel[2]*r->vel[2])) << std::endl;
	
}


