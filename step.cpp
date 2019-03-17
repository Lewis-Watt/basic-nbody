#include <iostream>
#include <vector>
#include <cmath>
#include "units.h"
#include "step.h"
#include "particle_data.h"



void acc(particle_data *r, double mass, particle_data **planet)
{
	/* Force components are the ratio of the axis/the modulus cubed of the radius
	*/
	double radius;
	radius = std::sqrt(r->pos[0]*r->pos[0] + r->pos[1]*r->pos[1] + r->pos[2]*r->pos[2]);
	for (int h=0; h<r->pos.capacity();h++)
	{

		r->a[h] = ((-grav_const*mass*r->pos[h])/(std::pow(radius,3)));

	}
	if (r->gravity ==true)
	{

		for (int o=0; o<tot_planets;o++)
		{
			particle_data *q = planet[o];
			double qr;
			qr = (std::sqrt((r->pos[0]-q->pos[0])*(r->pos[0]-q->pos[0]) + 
				(r->pos[1]-q->pos[1])*(r->pos[1]-q->pos[1]) + 
				(r->pos[2]-q->pos[2])*(r->pos[2]-q->pos[2])));
			for (int h=0; h<r->pos.capacity();h++)
			{
				r->a[h] += (-grav_const*q->mass*(r->pos[h]-q->pos[h]))/std::pow(qr,3);
			}
		}
	}

	else
	{
		for (int o=0; o<tot_planets;o++)
		{
			particle_data *q = planet[o];
			
			if (r->id==o)
			{
				continue;
			}
			double qr;
			qr = (std::sqrt((r->pos[0]-q->pos[0])*(r->pos[0]-q->pos[0]) + 
				(r->pos[1]-q->pos[1])*(r->pos[1]-q->pos[1]) + 
				(r->pos[2]-q->pos[2])*(r->pos[2]-q->pos[2])));
			for (int h=0; h<r->pos.capacity();h++)
			{
				r->a[h] += (-grav_const*q->mass*(-r->pos[h]+q->pos[h]))/std::pow(qr,3);
			}
		}
	}
}

void vel(particle_data *r, double t,double mass, particle_data **q)
{
	std::vector<double> a_before = {r->a[0],r->a[1],r->a[2]};
	acc(r,mass,q);
	for (int h=0; h < r->vel.capacity(); h++)
	{
		r->vel[h] = 0.5*(a_before[h]+r->a[h])*t + r->vel[h];
	}
}

void dist(particle_data *r,double t)
{
	for (int h=0; h < r->pos.capacity(); h++)
	{
		r->pos[h] = r->pos[h] + r->vel[h]*t +0.5*r->a[h]*t*t;
	}
}

void step(particle_data **r, int s, double step_size,  particle_data **q,double mass)
{
	if (r[0]->gravity==false)
	{
		for (int i=0; i<tot_planets; i++)
		{
			if (s==0)
			{
				acc(r[i],mass,q);
			}
			dist(r[i],step_size);
			vel(r[i],step_size,mass,q);
		}
	}
	else
	{
		for (int i=0; i<tot_particles; i++)
		{
			if (s==0)
			{
				acc(r[i],mass,q);
			}
			dist(r[i],step_size);
			vel(r[i],step_size,mass,q);
		}
	}

}