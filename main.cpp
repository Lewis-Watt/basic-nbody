#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include <fstream>
#include <sstream>
#include <cstring>
#include "units.h"
#include "step.h"
#include "open_data.h"
#include "init_particle.h"
#include "load.h"

/* TO DO:
1) Sort out units - Partially done
2) Add a second particle - done
3) Add gravity to second particle from first - done but will need to redo for general
   form.
4) Add parameter file inserts for grav and non-grav particle initial conditions - done
5) Figure out how to parallelise - Is not needed, but may be desired in the future
6) Add radiative force to particles
7) Added multiple gravitating bodies to the code
   -need to add gravity betweem them. DONE
8) Change initial variables to r,rp,e and true anomaly then resolve for x,y,z
   vx,vy,vz. Kinda Done - need to add them to the parameter file input
9)Need to orientate the inputted data
*/



int main(int argc, char* argv[])
{
	std::string param_file_name = argv[1];
	parameter_readin(param_file_name);
	/*
	This is where the particles are set up initial conditions. Will be replaced by
	3 files - a parameter file, particle data file, planet data file
	*/

	std::vector<particle_data*> test2 (tot_planets);
	particle_data **planets = test2.data();
	init_particle(planets,tot_planets);
	std::vector<particle_data*> test (tot_particles);
	particle_data **particles = test.data();
	init_particle(particles,tot_particles);

	for (int u=0;u<tot_planets;u++)
	{
		if (u==0)
		{
			initial_orbit_velocity(planets[u]);
		}
		else
			planets[u]->vel[0] = planets[0]->vel[0];
			planets[u]->vel[1] = planets[0]->vel[1];
			planets[u]->vel[2] = planets[0]->vel[2];
	}
	std::cout << "Passes vel set up" << std::endl;
	opendata(planets,planet_input);
	planets[0]->mass = 1*me;
	planets[1]->mass = 1*me;

	for (int u=0; u<tot_particles;u++)
	{
		particles[u]->pos[0] = planets[0]->pos[0];
		particles[u]->pos[1] = planets[0]->pos[1];
		particles[u]->pos[2] = planets[0]->pos[2];
		particles[u]->vel[0] = planets[0]->vel[0];
		particles[u]->vel[1] = planets[0]->vel[1];
		particles[u]->vel[2] = planets[0]->vel[2];
		particles[u]->mass = 1;
		particles[u]->gravity = true;
	}
	opendata(particles,particle_input);



	std::ofstream output_file;
	output_file.open (fout);
	for (int j=0; j<nsteps; j++)
	{
		step(particles,j,step_size*period,planets);
		step(planets,j,step_size*period,planets);
		if (j%stepout==0)
		{

			for (int g=0; g<tot_particles; g++)
			{
				output_file << particles[g]->pos[0] << "\t";
			}
			for (int g=0;g<tot_planets; g++)
			{
				output_file << planets[g]->pos[0] << "\t";
			}
			for (int g=0; g<tot_particles; g++)
			{
				output_file << particles[g]->pos[1] << "\t";
			}
			for (int g=0;g<tot_planets; g++)
			{
				output_file << planets[g]->pos[1] << "\t";
			}
			for (int g=0; g<tot_particles; g++)
			{
				output_file << particles[g]->pos[2] << "\t";
			}
			for (int g=0;g<tot_planets; g++)
			{
				output_file << planets[g]->pos[2] << "\t";
			}
			output_file << std::endl;
		}

	}
	output_file.close();
	return 0;
}

