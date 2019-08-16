#include <iostream>
#include <vector>
#include <cmath>
#include "units.h"
#include "step.h"
#include "particle_data.h"



void acc(particle_data *r, particle_data **planet)
{
	/* Force components are the ratio of the axis/the modulus cubed of the radius
	*/
	double radius;
	radius = std::sqrt(r->pos[0]*r->pos[0] + r->pos[1]*r->pos[1] + r->pos[2]*r->pos[2]);
	//std::cout << radius << std::endl;
	for (int h=0; h<r->pos.capacity();h++)
	{
		
		r->a[h] = ((-grav_const*star_mass*r->pos[h])/(radius*radius*radius));

	}

	if (r->gravity ==true)
	{

		for (int o=0; o<tot_planets;o++)
		{
			particle_data *q = planet[o];
			for (int h=0; h<r->pos.capacity();h++)
			{
				r->a[h] += (-grav_const*q->mass*(r->pos[h]-q->pos[h]))/(r->dtop[o]*r->dtop[o]*r->dtop[o]);
			}
		}
	}
// change back once finished without planet
	else
	{
		for (int o=0; o<tot_planets;o++)
		{
			particle_data *q = planet[o];
			
			if (r->id==q->id)
			{
				continue;
			}

			for (int h=0; h<r->pos.capacity();h++)
			{
				r->a[h] += (-grav_const*q->mass*(-r->pos[h]+q->pos[h]))/(r->dtop[o]*r->dtop[o]*r->dtop[o]);
			}
		}
	}
	
}

void vel(particle_data *r, double t, particle_data **q)
{
	std::vector<double> a_before = {r->a[0],r->a[1],r->a[2]};
	acc(r,q);
	for (int h=0; h < r->vel.capacity(); h++)
	{
		r->vel[h] += t*r->a[h];
	}
}

void dist(particle_data *r,double t,particle_data **q)
{

	for (int h=0; h < r->pos.capacity(); h++)
	{

		r->pos[h] += 0.5*t*r->vel[h];
	}
}

double dist_to_planet(particle_data *r, particle_data *q)
{
	double xdiff = r->pos[0]-q->pos[0];
	double ydiff = r->pos[1]-q->pos[1];
	double zdiff = r->pos[2]-q->pos[2];
	return std::sqrt((xdiff*xdiff) + (ydiff*ydiff) + (zdiff*zdiff));
}

double vel_to_planet(particle_data *r, particle_data *q)
{
	double xdiff = r->vel[0]-q->vel[0];
	double ydiff = r->vel[1]-q->vel[1];
	double zdiff = r->vel[2]-q->vel[2];
	return std::sqrt((xdiff*xdiff) + (ydiff*ydiff) + (zdiff*zdiff));
}


double escape_vel(particle_data *r, particle_data *q)
{
	double xdiff = r->pos[0]-q->pos[0];
	double ydiff = r->pos[1]-q->pos[1];
	double zdiff = r->pos[2]-q->pos[2];
	return std::sqrt(2*q->mass/q->hr); //std::sqrt((xdiff*xdiff) + (ydiff*ydiff) + (zdiff*zdiff)));
}

double hill_radius(particle_data *q)
{
	return (std::cbrt((q->mass/(3*(star_mass+q->mass)))))*sma;
}


void remove_particles(particle_data **r, particle_data **q)
{
	int count = 0;
	for (int p=0; p<tot_particles; p++)
	{
		if (r[p]->deleted == true)
		{
			continue;
		}
		for (int n=0; n<tot_planets; n++)
		{
			r[p]->dtop[n] = dist_to_planet(r[p],q[n]);
			if (r[p]->dtop[n] < 0*q[n]->planet_radius)
			{
				q[n]->mass += r[p]->mass;
				r[p]->deleted = true;
				break;
			}
		}
	}
	do{
		if (r[count]->deleted == true)
		{
			r[count] = r[tot_particles-1];
			//free(r[tot_particles-1]);
			tot_particles--;
			
		}
		else
		{
			count++;
		}
	}while(count!=tot_particles);
}


void select(particle_data *r, particle_data **q,double t)
{

	double radius = std::sqrt(r->pos[0]*r->pos[0] + r->pos[1]*r->pos[1] + r->pos[2]*r->pos[2]);
	double p_around_star = std::sqrt(4*M_PI*M_PI*radius*radius*radius/(grav_const*star_mass));
	if (p_around_star/100 < t)
	{
		r->scount +=1;
	}

	else
	{
		for (int m=0; m<tot_planets; m++)
			{	
				r->dtop[m] = dist_to_planet(r,q[m]);
				
				if (r->dtop[m]<q[m]->hr)
				{
					double p_around_planet = std::sqrt(4*M_PI*M_PI*r->dtop[m]*r->dtop[m]*r->dtop[m]/(grav_const*q[m]->mass));
					
					if (p_around_planet/20 < t)
					{
						
						r->scount +=1;
						break;
					}
				}
			}
	}
	
}

void step(particle_data **r, int s, double t,  particle_data **q)
{
	int max_it =0;
	int max_scount = 0;
	for (int p=0; p<tot_particles; p++)
	{	
		double st;
		double rad = std::sqrt(r[p]->pos[0]*r[p]->pos[0] + r[p]->pos[1]*r[p]->pos[1] + r[p]->pos[2]*r[p]->pos[2]);
		
		r[p]->scount = 0;
		dist(r[p],t,q);
		select(r[p],q,t);
		dist(r[p],-t,q);
		if (r[p]->scount!=0)
		{
			int sc_check;
			do
			{
				sc_check = r[p]->scount;
				
				st = t/std::pow(2,r[p]->scount);
				dist(r[p],st,q);
				select(r[p],q,st);
				dist(r[p],-st,q);

			}while(sc_check!=r[p]->scount && r[p]->scount<5);
		}
		if (max_it < std::pow(2,r[p]->scount))
		{
			max_it = std::pow(2,r[p]->scount);
			max_scount = r[p]->scount;
		}
		//std::cout << "scount: " << r[p]->scount;
	}
	
	double min_timestep = t/max_it;
	for (int m_it = 0; m_it<((max_it*4) -1); m_it++)
	{

		
		for (int p=0; p<tot_planets; p++)
		{	

            int diff_scount;
			diff_scount = 0;
			int vel_step_start = (std::pow(2,diff_scount)*2) - 1;
			int vel_skip = std::pow(2,diff_scount)*4;
			if (m_it%2==1)
			{
				for (int n=0; n<tot_planets; n++)
				{
					q[p]->dtop[n] = dist_to_planet(q[p],q[n]);
				}
				if (m_it%vel_skip==vel_step_start)
				{
					vel(q[p],min_timestep*std::pow(2,diff_scount),q);
				}
			}
			else
			{
				dist(q[p],min_timestep,q);
			}
                

		}

		for (int p=0; p<tot_particles; p++)
		{
			if (r[p]->deleted==true)
			{
				continue;
			}
			int diff_scount;
			diff_scount = max_scount - r[p]->scount;
			int vel_step_start = (std::pow(2,diff_scount)*2) - 1;
			int vel_skip = std::pow(2,diff_scount)*4;
			if (m_it%2==1)
			{
				
				for (int n=0; n<tot_planets; n++)
				{
					r[p]->dtop[n] = dist_to_planet(r[p],q[n]);
					if (r[p]->dtop[n] < 1.3*q[n]->planet_radius && s>(0.1*period/t))
					{
						q[n]->mass += r[p]->mass;
						r[p]->deleted = true;
						break;
					}
				}
				
				if (r[p]->deleted==true)
				{
					continue;
				}
				else if (m_it%vel_skip==vel_step_start)
				{
					vel(r[p],min_timestep*std::pow(2,diff_scount),q);
				}
			}
			else
			{
				dist(r[p],min_timestep,q);
			}
		}


	}
}
